/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "object_wavefront.h"
#include "object.h"
#include "object_valid.h"
#include <ctype.h>
#include "../chk/chk.h"
#include "../vector/vector.h"
#include "../string/string_numbers.h"
#include "../mli/mliDynMap.h"
#include "object_face_vector.h"
#include "../mli/mliDynVec.h"
#include "../mli/mliDynUint32.h"
#include "../mli/mliDynDouble.h"
#include "../io/io_text.h"
#include "../cstr/cstr.h"
#include "../cstr/cstr_numbers.h"
#include "../math/math.h"

#define MLI_WAVEFRONT_FACE_LINE_V 7
#define MLI_WAVEFRONT_FACE_LINE_V_VN 37
#define MLI_WAVEFRONT_FACE_LINE_V_VT_VN 25

#define MLI_WAVEFRONT_LINE_BUFF_LENGTH 64

int mliObject_is_face_line_toggle(const int state)
{
        switch (state) {
        case 3:
        case 5:
        case 7:

        case 27:
        case 29:
        case 32:
        case 34:
        case 37:

        case 11:
        case 13:
        case 15:
        case 17:
        case 19:
        case 21:
        case 23:
        case 25:
                return 1;
                break;
        }
        return 0;
}

int mli_String_to_uint32(uint32_t *out, const struct mli_String *str)
{
        uint64_t u = 0;
        chk(mli_String_to_uint64(&u, str, 10));
        (*out) = (uint32_t)u;
        return 1;
chk_error:
        return 0;
}

int mliObject_parse_face_line(
        const struct mli_String *line,
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
        const uint64_t MAX_NUM_CHARS = 256;
        uint64_t i = 0;
        char c;

        struct mli_String wuff = mli_String_init();
        chk(mli_String_malloc(&wuff, MAX_NUM_CHARS));
        wuff.size = 0;

        while (state != final_state) {
                chk_msg(i <= MAX_NUM_CHARS, "Expected less chars in line.");
                if (i < line->size) {
                        chk_msg(mli_String_get(line, i, &c), "Beyond 'line'.");
                } else {
                        c = '\0';
                }

                /* printf("i: %d, c:%c state: %d \n", i, c, state); */

                if (state == error_state) {
                        *line_mode = -1;
                        chk_bad("Can not parse line.");
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

                if (mliObject_is_face_line_toggle(state)) {
                        wuff.array[wuff.size] = c;
                        wuff.size++;
                } else if (mliObject_is_face_line_toggle(old_state)) {
                        uint64_t r;
                        for (r = wuff.size; r < MAX_NUM_CHARS; r++) {
                                wuff.array[r] = '\0';
                        }

                        switch (old_state) {
                        /* MLI_WAVEFRONT_FACE_LINE_V                          */
                        case 3:
                                chk(mli_String_to_uint32(&v->a, &wuff));
                                break;
                        case 5:
                                chk(mli_String_to_uint32(&v->b, &wuff));
                                break;
                        case 7:
                                chk(mli_String_to_uint32(&v->c, &wuff));
                                break;

                        /* MLI_WAVEFRONT_FACE_LINE_V_VN                       */
                        /*    3                        v->a                   */
                        case 27:
                                chk(mli_String_to_uint32(&vn->a, &wuff));
                                break;
                        case 29:
                                chk(mli_String_to_uint32(&v->b, &wuff));
                                break;
                        case 32:
                                chk(mli_String_to_uint32(&vn->b, &wuff));
                                break;
                        case 34:
                                chk(mli_String_to_uint32(&v->c, &wuff));
                                break;
                        case 37:
                                chk(mli_String_to_uint32(&vn->c, &wuff));
                                break;

                        /* MLI_WAVEFRONT_FACE_LINE_V_VT_VN                    */
                        /*    3                        v->a                   */
                        case 11:
                                chk(mli_String_to_uint32(&vt->a, &wuff));
                                break;
                        case 13:
                                chk(mli_String_to_uint32(&vn->a, &wuff));
                                break;
                        case 15:
                                chk(mli_String_to_uint32(&v->b, &wuff));
                                break;
                        case 17:
                                chk(mli_String_to_uint32(&vt->b, &wuff));
                                break;
                        case 19:
                                chk(mli_String_to_uint32(&vn->b, &wuff));
                                break;
                        case 21:
                                chk(mli_String_to_uint32(&v->c, &wuff));
                                break;
                        case 23:
                                chk(mli_String_to_uint32(&vt->c, &wuff));
                                break;
                        case 25:
                                chk(mli_String_to_uint32(&vn->c, &wuff));
                                break;

                        default:
                                break;
                        }
                        wuff.size = 0;
                }

                old_state = state;
                i++;
        }
        mli_String_free(&wuff);
        return 1;
chk_error:
        mli_String_free(&wuff);
        return 0;
}

int mliObject_is_vert_line_toggle(const int state)
{
        switch (state) {
        case 2:
        case 4:
        case 6:
                return 1;
                break;
        }
        return 0;
}

int mliObject_parse_three_float_line(
        const struct mli_String *line,
        struct mli_Vec *v)
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
        const uint64_t MAX_NUM_CHARS = 256;
        uint64_t i = 0;
        char c;

        struct mli_String wuff = mli_String_init();
        chk(mli_String_malloc(&wuff, MAX_NUM_CHARS));
        wuff.size = 0;

        while (state != final_state) {
                chk_msg(i <= MAX_NUM_CHARS, "Expected less chars in line.");
                if (i < line->size) {
                        chk_msg(mli_String_get(line, i, &c), "Beyond 'line'.");
                } else {
                        c = '\0';
                }

                if (state == error_state) {
                        fprintf(stderr,
                                "[ERROR] Can not parse line '%s'\n",
                                line->array);
                }
                chk_msg(state != error_state,
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

                if (mliObject_is_vert_line_toggle(state)) {
                        wuff.array[wuff.size] = c;
                        wuff.size++;
                } else if (mliObject_is_vert_line_toggle(old_state)) {
                        uint64_t r;
                        for (r = wuff.size; r < MAX_NUM_CHARS; r++) {
                                wuff.array[r] = '\0';
                        }

                        switch (old_state) {
                        case 2:
                                chk(mli_String_to_double(&v->x, &wuff));
                                break;
                        case 4:
                                chk(mli_String_to_double(&v->y, &wuff));
                                break;
                        case 6:
                                chk(mli_String_to_double(&v->z, &wuff));
                                break;
                        default:
                                break;
                        }
                        wuff.size = 0;
                }

                old_state = state;
                i++;
        }
        return 1;
chk_error:
        return 0;
}

int mliObject_parse_face_vertices_and_normals(
        const struct mli_String *line,
        struct mliFace *fv,
        struct mliFace *fvn)
{
        int line_mode = -1;
        struct mliFace tmp_fvt;

        chk_msg(mliObject_parse_face_line(line, fv, &tmp_fvt, fvn, &line_mode),
                "Can not parse face-line.");

        chk_msg((line_mode == MLI_WAVEFRONT_FACE_LINE_V_VT_VN) ||
                        (line_mode == MLI_WAVEFRONT_FACE_LINE_V_VN),
                "Expected faces to have vertex-normals.");

        chk_msg(fv->a >= 1, "Expected fv.a >= 1");
        chk_msg(fv->b >= 1, "Expected fv.b >= 1");
        chk_msg(fv->c >= 1, "Expected fv.c >= 1");
        fv->a -= 1;
        fv->b -= 1;
        fv->c -= 1;

        chk_msg(fvn->a >= 1, "Expected fvn.a >= 1");
        chk_msg(fvn->b >= 1, "Expected fvn.b >= 1");
        chk_msg(fvn->c >= 1, "Expected fvn.c >= 1");
        fvn->a -= 1;
        fvn->b -= 1;
        fvn->c -= 1;
        return 1;
chk_error:
        return 0;
}

int mliObject_malloc_from_wavefront(struct mliObject *obj, struct mli_IO *io)
{
        uint64_t i = 0u;
        uint64_t line_number = 0u;
        uint64_t mtl = 0u;

        struct mli_String line = mli_String_init();
        struct mli_String tmp = mli_String_init();
        struct mli_String svn = mli_String_init();
        struct mli_String sv = mli_String_init();
        struct mli_String sf = mli_String_init();
        struct mli_String susemtl = mli_String_init();

        /* init dyn */
        struct mliDynVec v = mliDynVec_init();
        struct mliDynVec vn = mliDynVec_init();

        struct mliDynFace fv = mliDynFace_init();
        struct mliDynFace fvn = mliDynFace_init();
        struct mliDynUint32 fm = mliDynUint32_init();

        struct mliDynMap material_names = mliDynMap_init();

        /* malloc dyn */
        chk(mliDynVec_malloc(&v, 0u));
        chk(mliDynVec_malloc(&vn, 0u));

        chk(mliDynFace_malloc(&fv, 0u));
        chk(mliDynFace_malloc(&fvn, 0u));
        chk(mliDynUint32_malloc(&fm, 0u));

        chk(mliDynMap_malloc(&material_names));

        chk(mli_String_from_cstr(&svn, "vn "));
        chk(mli_String_from_cstr(&sv, "v "));
        chk(mli_String_from_cstr(&sf, "f "));
        chk(mli_String_from_cstr(&susemtl, "usemtl "));

        chk_dbg;
        /* parse wavefront into dyn */
        while (1) {
                line_number += 1;
                chk_msg(line_number < 1000 * 1000 * 1000,
                        "Expected less than 1e9 lines in wavefront obj file. "
                        "Something went wrong.");

                if (mli_IO_eof(io)) {
                        break;
                }
                chk_msg(mli_IO_text_read_line(io, &line, '\n'),
                        "Can not read line.");
                chk_msg(line.size < 1024, "Expected line.size < 1024.");

                if (line.size > 0) {
                        if (mli_String_starts_with(&line, &svn)) {
                                struct mli_Vec tmp_vn;
                                chk(mli_String_copyn(
                                        &tmp, &line, 2, line.size - 2));
                                chk_msg(mliObject_parse_three_float_line(
                                                &tmp, &tmp_vn),
                                        "Can not parse vertex-normal-line.");
                                chk_msg(mli_Vec_dot(tmp_vn, tmp_vn) > 0.0,
                                        "vn can not be normalized.") tmp_vn =
                                        mli_Vec_normalized(tmp_vn);

                                chk(mliDynVec_push_back(&vn, tmp_vn));
                        } else if (mli_String_starts_with(&line, &sv)) {
                                struct mli_Vec tmp_v;
                                chk(mli_String_copyn(
                                        &tmp, &line, 1, line.size - 1));
                                chk_msg(mliObject_parse_three_float_line(
                                                &tmp, &tmp_v),
                                        "Can not parse vertex-line.");
                                chk(mliDynVec_push_back(&v, tmp_v));
                        } else if (mli_String_starts_with(&line, &sf)) {
                                struct mliFace tmp_fv;
                                struct mliFace tmp_fvn;
                                chk_msg(mliDynMap_size(&material_names) > 0,
                                        "Expected 'usemtl' before first "
                                        "face 'f'.");
                                chk_msg(mliObject_parse_face_vertices_and_normals(
                                                &line, &tmp_fv, &tmp_fvn),
                                        "Failed to parse face-line.");
                                chk(mliDynFace_push_back(&fv, tmp_fv));
                                chk(mliDynFace_push_back(&fvn, tmp_fvn));
                                chk(mliDynUint32_push_back(&fm, mtl));
                        } else if (mli_String_starts_with(&line, &susemtl)) {
                                chk(mli_String_copyn(
                                        &tmp, &line, 7, line.size - 7));
                                if (!mliDynMap_has(&material_names, &tmp)) {
                                        chk(mliDynMap_insert(
                                                &material_names, &tmp, 0));
                                }
                                chk(mliDynMap_find(
                                        &material_names, &tmp, &mtl));
                        } else {
                                /*fprintf(stderr, "no match: '%s'\n",
                                 * line.array);*/
                        }
                } /* line_length > 0 */
        }
        chk_dbg

                /* copy dyn into static mliObject */
                chk_msg(fv.size == fvn.size,
                        "Expected num. vertex-indices == num. "
                        "vertex-normal-indices.");
        chk_msg(mliObject_malloc(
                        obj,
                        v.size,
                        vn.size,
                        fv.size,
                        mliDynMap_size(&material_names)),
                "Failed to malloc mliObject from file.");
        chk_dbg MLI_MATH_NCPY(v.array, obj->vertices, v.size);
        MLI_MATH_NCPY(vn.array, obj->vertex_normals, vn.size);
        MLI_MATH_NCPY(fv.array, obj->faces_vertices, fv.size);
        MLI_MATH_NCPY(fvn.array, obj->faces_vertex_normals, fvn.size);
        MLI_MATH_NCPY(fm.array, obj->faces_materials, fm.size);
        for (i = 0; i < mliDynMap_size(&material_names); i++) {
                chk(mli_String_copy(
                        &obj->material_names[i],
                        &material_names.items.array[i].key));
        }

        chk_msg(mliObject_is_valid(obj), "Expected object to be valid.");

        /* free dyn */
        mliDynVec_free(&v);
        mliDynVec_free(&vn);

        mliDynFace_free(&fv);
        mliDynFace_free(&fvn);
        mliDynUint32_free(&fm);

        mliDynMap_free(&material_names);

        mli_String_free(&line);
        mli_String_free(&tmp);
        mli_String_free(&svn);
        mli_String_free(&sv);
        mli_String_free(&sf);
        mli_String_free(&susemtl);

        return 1;
chk_error:
        mliObject_free(obj);

        /* free dyn */
        mliDynVec_free(&v);
        mliDynVec_free(&vn);

        mliDynFace_free(&fv);
        mliDynFace_free(&fvn);
        mliDynUint32_free(&fm);

        mliDynMap_free(&material_names);

        mli_String_free(&line);
        mli_String_free(&tmp);
        mli_String_free(&svn);
        mli_String_free(&sv);
        mli_String_free(&sf);
        mli_String_free(&susemtl);

        return 0;
}

int mliObject_fprint_to_wavefront(struct mli_IO *f, const struct mliObject *obj)
{
        uint32_t i, mtl, face;
        chk(mli_IO_text_write_cstr_format(f, "# vertices\n"));
        for (i = 0; i < obj->num_vertices; i++) {
                chk(mli_IO_text_write_cstr_format(
                        f,
                        "v %.6f %.6f %.6f\n",
                        obj->vertices[i].x,
                        obj->vertices[i].y,
                        obj->vertices[i].z));
        }

        chk(mli_IO_text_write_cstr_format(f, "# vertex normals\n"));
        for (i = 0; i < obj->num_vertex_normals; i++) {
                chk(mli_IO_text_write_cstr_format(
                        f,
                        "vn %.6f %.6f %.6f\n",
                        obj->vertex_normals[i].x,
                        obj->vertex_normals[i].y,
                        obj->vertex_normals[i].z));
        }

        chk(mli_IO_text_write_cstr_format(f, "# faces\n"));
        for (face = 0; face < obj->num_faces; face++) {
                if ((face == 0) || (mtl != obj->faces_materials[face])) {
                        mtl = obj->faces_materials[face];
                        chk(mli_IO_text_write_cstr_format(
                                f,
                                "usemtl %s\n",
                                obj->material_names[mtl].array));
                }

                chk(mli_IO_text_write_cstr_format(
                        f,
                        "f %d//%d %d//%d %d//%d\n",
                        obj->faces_vertices[face].a + 1,
                        obj->faces_vertex_normals[face].a + 1,
                        obj->faces_vertices[face].b + 1,
                        obj->faces_vertex_normals[face].b + 1,
                        obj->faces_vertices[face].c + 1,
                        obj->faces_vertex_normals[face].c + 1));
        }

        return 1;
chk_error:
        return 0;
}
