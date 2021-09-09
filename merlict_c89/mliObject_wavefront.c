/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliObject_wavefront.h"
#include "mli_debug.h"
#include "mliDynArray_template.h"
#include <ctype.h>

#define MLI_WAVEFRONT_FACE_LINE_V 7
#define MLI_WAVEFRONT_FACE_LINE_V_VN 37
#define MLI_WAVEFRONT_FACE_LINE_V_VT_VN 25

#define MLI_WAVEFRONT_LINE_BUFF_LENGTH 64

struct _mliBuff {
        int length;
        int b;
        char buff[MLI_WAVEFRONT_LINE_BUFF_LENGTH];
};

struct _mliBuff _mliBuff_init(void)
{
        int i = 0;
        struct _mliBuff bb;
        bb.length = MLI_WAVEFRONT_LINE_BUFF_LENGTH;
        bb.b = 0;
        for (i = 0; i < bb.length; i++) {
                bb.buff[i] = '\0';
        }
        return bb;
}

int mliBuff_to_uint(
        const char c,
        struct _mliBuff *buff,
        const int toggle_State,
        const int state,
        const int old_state,
        uint32_t *out)
{
        int64_t tmp = -1;
        chk_msg(
                buff->b < MLI_WAVEFRONT_LINE_BUFF_LENGTH,
                "Integer-buff is full.");
        if (state == toggle_State) {
                buff->buff[buff->b] = c;
                buff->b++;
        }
        if (old_state == toggle_State && state != toggle_State) {
                for (; buff->b < buff->length; buff->b++) {
                        buff->buff[buff->b] = '\0';
                }
                buff->b = 0;
                chk_msg(
                        mli_string_to_int(&tmp, buff->buff, 10),
                        "Can not parse face index");
                chk_msg(tmp > 0, "Expected object's index > 0.");
                *out = tmp;
        }

        return 1;
error:
        return 0;
}

int mliBuff_to_double(
        const char c,
        struct _mliBuff *buff,
        const int toggle_State,
        const int state,
        const int old_state,
        double *out)
{
        chk_msg(
                buff->b < MLI_WAVEFRONT_LINE_BUFF_LENGTH,
                "Integer-buff is full.");
        if (state == toggle_State) {
                buff->buff[buff->b] = c;
                buff->b++;
        }
        if (old_state == toggle_State && state != toggle_State) {
                for (; buff->b < buff->length; buff->b++) {
                        buff->buff[buff->b] = '\0';
                }
                buff->b = 0;
                chk_msg(
                        mli_string_to_float(out, buff->buff),
                        "Can not parse face index");
        }

        return 1;
error:
        return 0;
}

int _mliObject_parse_face_line(
        const char *line,
        struct mliFace *faces_vertices,
        struct mliFace *faces_texture_points,
        struct mliFace *faces_vertex_normals,
        int *line_mode)
{
        struct mliFace *v = faces_vertices;
        struct mliFace *vt = faces_texture_points;
        struct mliFace *vn = faces_vertex_normals;
        /*
        statemachine
        ============

        ( 0)
         |
         | 'f'
         V
        ( 1)
         |
         | ws
         V  ___
        ( 2)<__] ws
         |
         | dig
         V  ___
        ( 3)<__] dig
         |\________________________
         | ws                      | '/'
         V  ___                    V
        ( 4)<__] ws               (10)
         |                         |\___________________________
         | dig                     | dig                        | '/'
         V  ___                    V  ___                       V
        ( 5)<__] dig              (11)<__] dig                 (26)
         |                         |                            |
         | ws                      | '/'                        | dig
         V  ___                    V                            V  ___
        ( 6)<__] ws               (12)                         (27)<__] dig
         |                         |                            |
         | dig                     | dig                        | ws
         V  ___                    V  ___                       V  ___
        ( 7)<__] dig              (13)<__] dig                 (28)<__] ws
         |                         |                            |
         | !dig OR ws              | ws                         | dig
         V                         V  ___                       V  ___
        (END)                     (14)<__] ws                  (29)<__] dig
                                   |                            |
        f v v v                    | dig                        | '/'
                                   V  ___                       V
                                  (15)<__] dig                 (30)
                                   |                            |
                                   | '/'                        | '/'
                                   V                            V
                                  (16)                         (31)
                                   |                            |
                                   | dig                        | dig
                                   V  ___                       V  ___
                                  (17)<__] dig                 (32)<__] dig
                                   |                            |
                                   | '/'                        | ws
                                   V                            V  ___
                                  (18)                         (33)<__] ws
                                   |                            |
                                   | dig                        | dig
                                   V  ___                       V  ___
                                  (19)<__] dig                 (34)<__] dig
                                   |                            |
                                   | ws                         | '/'
                                   V  ___                       V
                                  (20)<__] ws                  (35)
                                   |                            |
                                   | dig                        | '/'
                                   V  ___                       V
                                  (21)<__] dig                 (36)
                                   |                            |
                                   | '/'                        | dig
                                   V                            V  ___
                                  (22)                         (37)<__] dig
                                   |                            |
                                   | dig                        | !dig OR ws
                                   V  ___                       V
                                  (23)<__] dig                 (END)
                                   |
                                   | '/'                 f v//vn v//vn v//vn
                                   V
                                  (24)
                                   |
                                   | dig
                                   V  ___
                                  (25)<__] dig
                                   |
                                   | !dig OR ws
                                   V
                                  (END)

                                f v/vt/vn v/vt/vn v/vt/vn

        */
        const int final_state = 99;
        const int error_state = -1;
        int statemachine[][5] = {
                /*       'f'  ws  dig !dig '/' */
                {01, -1, -1, -1, -1}, /* 00 */

                {-1, 02, -1, -1, -1}, /* 01 */
                {-1, 02, 03, -1, -1}, /* 02 */
                {-1, 04, 03, -1, 10}, /* 03 */
                {-1, 04, 05, -1, -1}, /* 04 */
                {-1, 06, 05, -1, -1}, /* 05 */
                {-1, 06, 07, -1, -1}, /* 06 */
                {-1, 99, 07, 99, -1}, /* 07 */

                {-1, -1, -1, -1, -1}, /* 08 */
                {-1, -1, -1, -1, -1}, /* 09 */

                {-1, -1, 11, -1, 26}, /* 10 */
                {-1, -1, 11, -1, 12}, /* 11 */
                {-1, -1, 13, -1, -1}, /* 12 */
                {-1, 14, 13, -1, -1}, /* 13 */
                {-1, 14, 15, -1, -1}, /* 14 */
                {-1, -1, 15, -1, 16}, /* 15 */
                {-1, -1, 17, -1, -1}, /* 16 */
                {-1, -1, 17, -1, 18}, /* 17 */
                {-1, -1, 19, -1, -1}, /* 18 */
                {-1, 20, 19, -1, -1}, /* 19 */
                {-1, 20, 21, -1, -1}, /* 20 */
                {-1, -1, 21, -1, 22}, /* 21 */
                {-1, -1, 23, -1, -1}, /* 22 */
                {-1, -1, 23, -1, 24}, /* 23 */
                {-1, -1, 25, -1, -1}, /* 24 */
                {-1, 99, 25, 99, -1}, /* 25 */

                {-1, -1, 27, -1, -1}, /* 26 */
                {-1, 28, 27, -1, -1}, /* 27 */
                {-1, 28, 29, -1, -1}, /* 28 */
                {-1, -1, 29, -1, 30}, /* 29 */
                {-1, -1, -1, -1, 31}, /* 30 */
                {-1, -1, 32, -1, -1}, /* 31 */
                {-1, 33, 32, -1, -1}, /* 32 */
                {-1, 33, 34, -1, -1}, /* 33 */
                {-1, -1, 34, -1, 35}, /* 34 */
                {-1, -1, -1, -1, 36}, /* 35 */
                {-1, -1, 37, -1, -1}, /* 36 */
                {-1, 99, 37, 99, -1}, /* 37 */
        };

        int state = 0;
        int old_state = state;
        const int MAX_NUM_CHARS = 256;
        int i = 0;
        char c;

        struct _mliBuff buff = _mliBuff_init();

        while (state != final_state) {
                chk_msg(i <= MAX_NUM_CHARS, "Expected less chars in line.");
                c = line[i];

                /* printf("i: %d, c:%c state: %d \n", i, c, state); */

                if (state == error_state) {
                        *line_mode = -1;
                        chk_sentinel("Can not parse line.");
                }

                /* next state */
                /* ========== */
                if (c == 'f') {
                        state = statemachine[state][0];
                } else if (c == ' ') {
                        state = statemachine[state][1];
                } else if (c == '/') {
                        state = statemachine[state][4];
                } else if (isdigit(c)) {
                        state = statemachine[state][2];
                } else if (!isdigit(c)) {
                        state = statemachine[state][3];
                } else {
                        state = error_state;
                }

                /* line mode */
                /* ========= */
                if (state == MLI_WAVEFRONT_FACE_LINE_V) {
                        *line_mode = MLI_WAVEFRONT_FACE_LINE_V;
                }
                if (state == MLI_WAVEFRONT_FACE_LINE_V_VN) {
                        *line_mode = MLI_WAVEFRONT_FACE_LINE_V_VN;
                }
                if (state == MLI_WAVEFRONT_FACE_LINE_V_VT_VN) {
                        *line_mode = MLI_WAVEFRONT_FACE_LINE_V_VT_VN;
                }
                /* mode MLI_WAVEFRONT_FACE_LINE_V */

                chk(mliBuff_to_uint(c, &buff, 3, state, old_state, &v->a));
                chk(mliBuff_to_uint(c, &buff, 5, state, old_state, &v->b));
                chk(mliBuff_to_uint(c, &buff, 7, state, old_state, &v->c));

                /* mode MLI_WAVEFRONT_FACE_LINE_V_VN */

                /*chk(mliBuff_to_uint(c, &buff, 3, state, old_state,
                 * &v->a));*/
                chk(mliBuff_to_uint(c, &buff, 27, state, old_state, &vn->a));

                chk(mliBuff_to_uint(c, &buff, 29, state, old_state, &v->b));
                chk(mliBuff_to_uint(c, &buff, 32, state, old_state, &vn->b));

                chk(mliBuff_to_uint(c, &buff, 34, state, old_state, &v->c));
                chk(mliBuff_to_uint(c, &buff, 37, state, old_state, &vn->c));

                /* mode MLI_WAVEFRONT_FACE_LINE_V_VT_VN */

                /*chk(mliBuff_to_uint(c, &buff, 3, state, old_state,
                 * &v->a));*/
                chk(mliBuff_to_uint(c, &buff, 11, state, old_state, &vt->a));
                chk(mliBuff_to_uint(c, &buff, 13, state, old_state, &vn->a));

                chk(mliBuff_to_uint(c, &buff, 15, state, old_state, &v->b));
                chk(mliBuff_to_uint(c, &buff, 17, state, old_state, &vt->b));
                chk(mliBuff_to_uint(c, &buff, 19, state, old_state, &vn->b));

                chk(mliBuff_to_uint(c, &buff, 21, state, old_state, &v->c));
                chk(mliBuff_to_uint(c, &buff, 23, state, old_state, &vt->c));
                chk(mliBuff_to_uint(c, &buff, 25, state, old_state, &vn->c));

                old_state = state;
                i++;
        }
        return 1;
error:
        return 0;
}

int _mliObject_parse_three_float_line(const char *line, struct mliVec *v)
{
        /*
        statemachine
        ============

        ( 0)
         |
         | ws
         V  ___
        ( 1)<__] ws
         |
         | print
         V  ___
        ( 2)<__] print
         |
         | ws
         V  ___
        ( 3)<__] ws
         |
         | print
         V  ___
        ( 4)<__] print
         |
         | ws
         V  ___
        ( 5)<__] ws
         |
         | print
         V  ___
        ( 6)<__] print
         |
         | ws OR eol
         V
        (END)

        */
        const int final_state = 99;
        const int error_state = -1;
        int statemachine[][3] = {
                /* ws print eol */
                {01, -1, -1}, /* 0 */
                {01, 02, -1}, /* 1 */
                {03, 02, -1}, /* 2 */
                {03, 04, -1}, /* 3 */
                {05, 04, -1}, /* 4 */
                {05, 06, -1}, /* 5 */
                {99, 06, 99}, /* 6 */
        };

        int state = 0;
        int old_state = state;
        const int MAX_NUM_CHARS = 256;
        int i = 0;
        char c;

        struct _mliBuff buff = _mliBuff_init();

        while (state != final_state) {
                chk_msg(i <= MAX_NUM_CHARS, "Expected less chars in line.");
                c = line[i];

                if (state == error_state) {
                        fprintf(stderr,
                                "[ERROR] Can not parse line '%s'\n",
                                line);
                }
                chk_msg(
                        state != error_state,
                        "Can not parse three float line.");

                /* next state */
                /* ========== */
                if (c == ' ') {
                        state = statemachine[state][0];
                } else if (isprint(c)) {
                        state = statemachine[state][1];
                } else if (c == '\0') {
                        state = statemachine[state][2];
                } else {
                        state = error_state;
                }

                chk(mliBuff_to_double(c, &buff, 2, state, old_state, &v->x));
                chk(mliBuff_to_double(c, &buff, 4, state, old_state, &v->y));
                chk(mliBuff_to_double(c, &buff, 6, state, old_state, &v->z));

                old_state = state;
                i++;
        }
        return 1;
error:
        return 0;
}

int _mli_line_is_usemtl(const char *line, const uint64_t line_length)
{
        return line_length > 6 && line[0] == 'u' && line[1] == 's' &&
               line[2] == 'e' && line[3] == 'm' && line[4] == 't' &&
               line[5] == 'l' && line[6] == ' ';
}

int mliObject_malloc_from_wavefront(struct mliObject *obj, const char *str)
{
        uint64_t i = 0u;
        uint64_t p = 0u;
        uint64_t line_number = 0u;
        char line[2 * MLI_NAME_CAPACITY];
        uint64_t line_length = 0u;
        uint64_t mtl = 0u;
        const uint64_t debug_line_radius = 5u;

        /* init dyn */
        struct mliDynVec v = mliDynVec_init();
        struct mliDynVec vn = mliDynVec_init();

        struct mliDynFace fv = mliDynFace_init();
        struct mliDynFace fvn = mliDynFace_init();
        struct mliDynUint32 fm = mliDynUint32_init();

        struct mliDynMap material_names = mliDynMap_init();

        memset(line, '\0', sizeof(line));

        /* malloc dyn */
        chk(mliDynVec_malloc(&v, 0u));
        chk(mliDynVec_malloc(&vn, 0u));

        chk(mliDynFace_malloc(&fv, 0u));
        chk(mliDynFace_malloc(&fvn, 0u));
        chk(mliDynUint32_malloc(&fm, 0u));

        chk(mliDynMap_malloc(&material_names, 0u));

        /* parse wavefront into dyn */
        while (1) {
                line_number += 1;
                chk_msg(
                        line_number < 1000 * 1000 * 1000,
                        "Expected less than 1e9 lines in wavefront-file. "
                        "Something went wrong.");

                line_length =
                        mli_string_split(&str[p], '\n', line, sizeof(line));

                chk_msg(line_length < sizeof(line), "Line is too long.");

                if (line_length > 0) {
                        if (line[0] == 'v' && line[1] == 'n' &&
                            line[2] == ' ') {
                                /* vertex-normal-line*/
                                struct mliVec tmp_vn;
                                chk_msg(
                                        _mliObject_parse_three_float_line(
                                                &line[2], &tmp_vn),
                                        "Can not parse vertex-normal-line.");
                                chk(mliDynVec_push_back(&vn, tmp_vn));
                        } else if (line[0] == 'v' && line[1] == ' ') {
                                /* vertex line */
                                struct mliVec tmp_v;
                                chk_msg(
                                        _mliObject_parse_three_float_line(
                                                &line[1], &tmp_v),
                                        "Can not parse vertex-line.");
                                chk(mliDynVec_push_back(&v, tmp_v));
                        } else if (line[0] == 'f' && line[1] == ' ') {
                                /* face-line */
                                int line_mode = -1;
                                struct mliFace tmp_fv;
                                struct mliFace tmp_fvt;
                                struct mliFace tmp_fvn;

                                chk_msg(
                                        material_names.size > 0,
                                        "Expected 'usemtl' before first "
                                        "face 'f'.");

                                chk_msg(
                                        _mliObject_parse_face_line(
                                                line,
                                                &tmp_fv,
                                                &tmp_fvt,
                                                &tmp_fvn,
                                                &line_mode),
                                        "Can not parse face-line.");

                                chk_msg(tmp_fv.a >= 1, "Expected fv.a >= 1");
                                chk_msg(tmp_fv.b >= 1, "Expected fv.b >= 1");
                                chk_msg(tmp_fv.c >= 1, "Expected fv.c >= 1");
                                tmp_fv.a -= 1;
                                tmp_fv.b -= 1;
                                tmp_fv.c -= 1;

                                chk_msg(
                                        tmp_fvn.a >= 1, "Expected fvn.a >= 1");
                                chk_msg(
                                        tmp_fvn.b >= 1, "Expected fvn.b >= 1");
                                chk_msg(
                                        tmp_fvn.c >= 1, "Expected fvn.c >= 1");
                                tmp_fvn.a -= 1;
                                tmp_fvn.b -= 1;
                                tmp_fvn.c -= 1;

                                chk(mliDynFace_push_back(&fv, tmp_fv));
                                chk(mliDynFace_push_back(&fvn, tmp_fvn));
                                chk(mliDynUint32_push_back(&fm, mtl));
                                chk_msg(
                                        (line_mode ==
                                         MLI_WAVEFRONT_FACE_LINE_V_VT_VN) ||
                                                (line_mode ==
                                                 MLI_WAVEFRONT_FACE_LINE_V_VN),
                                        "Expected faces to have "
                                        "vertex-normals.");
                        } else if (_mli_line_is_usemtl(line, line_length)) {
                                const char *mtl_key = &line[7];
                                if (!mliDynMap_has(&material_names, mtl_key)) {
                                        chk(mliDynMap_insert(
                                                &material_names, mtl_key, 0));
                                }
                                chk(mliDynMap_find(
                                        &material_names, mtl_key, &mtl));
                        }
                } /* line_length > 0 */

                if (str[p + line_length] == '\0') {
                        break;
                }
                p += line_length + 1;
        }

        /* copy dyn into static mliObject */
        chk_msg(
                fv.size == fvn.size,
                "Expected num. vertex-indices == num. vertex-normal-indices.");
        chk_msg(
                mliObject_malloc(
                        obj, v.size, vn.size, fv.size, material_names.size),
                "Failed to malloc mliObject from file.");

        for (i = 0; i < v.size; i++) {
                obj->vertices[i] = v.array[i];
        }
        for (i = 0; i < vn.size; i++) {
                obj->vertex_normals[i] = mliVec_normalized(vn.array[i]);
        }
        for (i = 0; i < fv.size; i++) {
                obj->faces_vertices[i] = fv.array[i];
        }
        for (i = 0; i < fvn.size; i++) {
                obj->faces_vertex_normals[i] = fvn.array[i];
        }
        for (i = 0; i < fvn.size; i++) {
                obj->faces_materials[i] = fm.array[i];
        }
        for (i = 0; i < material_names.size; i++) {
                memcpy(obj->material_names[i].c_str,
                       material_names.array[i].key,
                       MLI_NAME_CAPACITY);
        }

        chk_msg(mliObject_is_valid(obj), "Expected object to be valid.");

        /* free dyn */
        mliDynVec_free(&v);
        mliDynVec_free(&vn);

        mliDynFace_free(&fv);
        mliDynFace_free(&fvn);
        mliDynUint32_free(&fm);

        mliDynMap_free(&material_names);

        return 1;
error:
        mliObject_free(obj);

        /* free dyn */
        mliDynVec_free(&v);
        mliDynVec_free(&vn);

        mliDynFace_free(&fv);
        mliDynFace_free(&fvn);
        mliDynUint32_free(&fm);

        mliDynMap_free(&material_names);

        if (MLI_PRINT_LEVEL) {
                mli_lines_info_fprint(
                        stderr, str, line_number, debug_line_radius);
        }

        return 0;
}

int mliObject_fprint_to_wavefront(FILE *f, const struct mliObject *obj)
{
        uint32_t i, mtl, face;
        chk(fprintf(f, "# vertices\n"));
        for (i = 0; i < obj->num_vertices; i++) {
                chk(
                        fprintf(f,
                                "v %.6f %.6f %.6f\n",
                                obj->vertices[i].x,
                                obj->vertices[i].y,
                                obj->vertices[i].z));
        }

        chk(fprintf(f, "# vertex normals\n"));
        for (i = 0; i < obj->num_vertex_normals; i++) {
                chk(
                        fprintf(f,
                                "vn %.6f %.6f %.6f\n",
                                obj->vertex_normals[i].x,
                                obj->vertex_normals[i].y,
                                obj->vertex_normals[i].z));
        }

        chk(fprintf(f, "# faces\n"));
        for (face = 0; face < obj->num_faces; face++) {
                if ((face == 0) || (mtl != obj->faces_materials[face])) {
                        mtl = obj->faces_materials[face];
                        chk(
                                fprintf(f,
                                        "usemtl %s\n",
                                        obj->material_names[mtl].c_str));
                }

                chk(
                        fprintf(f,
                                "f %d//%d %d//%d %d//%d\n",
                                obj->faces_vertices[face].a + 1,
                                obj->faces_vertex_normals[face].a + 1,
                                obj->faces_vertices[face].b + 1,
                                obj->faces_vertex_normals[face].b + 1,
                                obj->faces_vertices[face].c + 1,
                                obj->faces_vertex_normals[face].c + 1));
        }

        return 1;
error:
        return 0;
}
