/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliObject.h"
#include "mli_debug.h"

struct mliObject mliObject_init(void)
{
        struct mliObject obj;
        obj.num_vertices = 0;
        obj.vertices = NULL;

        obj.num_vertex_normals = 0;
        obj.vertex_normals = NULL;

        obj.num_faces = 0;
        obj.faces_vertices = NULL;
        obj.faces_vertex_normals = NULL;

        return obj;
}

void mliObject_free(struct mliObject *obj)
{
        free(obj->vertices);
        free(obj->vertex_normals);
        free(obj->faces_vertices);
        free(obj->faces_vertex_normals);
        *obj = mliObject_init();
}

int mliObject_malloc(
        struct mliObject *obj,
        const uint64_t num_vertices,
        const uint64_t num_vertex_normals,
        const uint64_t num_faces)
{
        mli_check(num_vertices < UINT32_MAX, "Expected num_vertices < uint32");
        mli_check(num_vertex_normals < UINT32_MAX, "Expected num_vertices < uint32");
        mli_check(num_faces < UINT32_MAX, "Expected num_vertices < uint32");

        mliObject_free(obj);
        obj->num_vertices = num_vertices;
        obj->num_vertex_normals = num_vertex_normals;
        obj->num_faces = num_faces;
        mli_malloc(obj->vertices, struct mliVec, obj->num_vertices);
        mli_malloc(obj->vertex_normals, struct mliVec, obj->num_vertex_normals);
        mli_malloc(obj->faces_vertices, struct mliFace, obj->num_faces);
        mli_malloc(obj->faces_vertex_normals, struct mliFace, obj->num_faces);
        return 1;
error:
        return 0;
}

int mliObject_assert_valid_faces(const struct mliObject *obj)
{
        uint64_t i;
        for (i = 0; i < obj->num_faces; i++) {
                mli_check(
                        obj->faces_vertices[i].a <= obj->num_vertices,
                        "Expected face.a <= num_vertices");
                mli_check(
                        obj->faces_vertices[i].b <= obj->num_vertices,
                        "Expected face.b <= num_vertices");
                mli_check(
                        obj->faces_vertices[i].c <= obj->num_vertices,
                        "Expected face.c <= num_vertices");

                mli_check(
                        obj->faces_vertex_normals[i].a <= obj->num_vertex_normals,
                        "Expected faces_vertex_normals.a <= num_vertex_normals");
                mli_check(
                        obj->faces_vertex_normals[i].b <= obj->num_vertex_normals,
                        "Expected faces_vertex_normals.b <= num_vertex_normals");
                mli_check(
                        obj->faces_vertex_normals[i].c <= obj->num_vertex_normals,
                        "Expected faces_vertex_normals.c <= num_vertex_normals");
        }
        return 1;
error:
        return 0;
}

#define MLI_WAVEFRONT_FACE_LINE_V 7
#define MLI_WAVEFRONT_FACE_LINE_V_VN 37
#define MLI_WAVEFRONT_FACE_LINE_V_VT_VN 25

#define _MLI_BUFF_LENGTH 64

struct _mliBuff {
        int length;
        int b;
        char buff[_MLI_BUFF_LENGTH];
};

struct _mliBuff _mliBuff_init(void) {
        int i = 0;
        struct _mliBuff bb;
        bb.length = _MLI_BUFF_LENGTH;
        bb.b = 0;
        for (i = 0; i < bb.length; i++) {
                bb.buff[i] = '\0';
        }
        return bb;
}


int _mli_fill_buff(
        const char c,
        struct _mliBuff *buff,
        const int toggle_State,
        const int state,
        const int old_state,
        uint32_t *out)
{
        int64_t tmp = -1;
        mli_check(buff->b < _MLI_BUFF_LENGTH, "Integer-buff is full.");
        if (state == toggle_State) {
                buff->buff[buff->b] = c;
                buff->b++;
        }
        if (old_state == toggle_State && state != toggle_State) {
                for (; buff->b < buff->length; buff->b++) {
                        buff->buff[buff->b] = '\0';
                }
                buff->b = 0;
                mli_check(
                        mli_string_to_int(&tmp, buff->buff, 10),
                        "Can not parse face index"
                );
                mli_check(tmp > 0, "Expected object's index > 0.")
                *out = tmp;
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
        /*

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
                                   | dig                        | '/'
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
                                   | '/'
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

        */
        const int FI = 99;
        int statemachine[][5] = {
        /*       'f'  ws  dig !dig '/' */
                { 01, -1, -1, -1, -1}, /* 00 */

                { -1, 02, -1, -1, -1}, /* 01 */
                { -1, 02, 03, -1, -1}, /* 02 */
                { -1, 04, 03, -1, 10}, /* 03 */
                { -1, 04, 05, -1, -1}, /* 04 */
                { -1, 06, 05, -1, -1}, /* 05 */
                { -1, 06, 07, -1, -1}, /* 06 */
                { -1, 99, 07, 99, -1}, /* 07 */

                { -1, -1, -1, -1, -1}, /* 08 */
                { -1, -1, -1, -1, -1}, /* 09 */

                { -1, -1, 11, -1, 26}, /* 10 */
                { -1, -1, 11, -1, 12}, /* 11 */
                { -1, -1, 13, -1, -1}, /* 12 */
                { -1, 14, 13, -1, -1}, /* 13 */
                { -1, 14, 15, -1, -1}, /* 14 */
                { -1, -1, 15, -1, 16}, /* 15 */
                { -1, -1, 17, -1, -1}, /* 16 */
                { -1, -1, 17, -1, 18}, /* 17 */
                { -1, -1, 19, -1, -1}, /* 18 */
                { -1, 20, 19, -1, -1}, /* 19 */
                { -1, 20, 21, -1, -1}, /* 20 */
                { -1, -1, 21, -1, 22}, /* 21 */
                { -1, -1, 23, -1, -1}, /* 22 */
                { -1, -1, 23, -1, 24}, /* 23 */
                { -1, -1, 25, -1, -1}, /* 24 */
                { -1, 99, 25, 99, -1}, /* 25 */

                { -1, -1, 27, -1, -1}, /* 26 */
                { -1, 28, 27, -1, -1}, /* 27 */
                { -1, 28, 29, -1, -1}, /* 28 */
                { -1, -1, 29, -1, 30}, /* 29 */
                { -1, -1, -1, -1, 31}, /* 30 */
                { -1, -1, 32, -1, -1}, /* 31 */
                { -1, 33, 32, -1, -1}, /* 32 */
                { -1, 33, 34, -1, -1}, /* 33 */
                { -1, -1, 34, -1, 35}, /* 34 */
                { -1, -1, -1, -1, 36}, /* 35 */
                { -1, -1, 37, -1, -1}, /* 36 */
                { -1, 99, 37, 99, -1}, /* 37 */
        };

        int state = 0;
        int old_state = state;
        const int MAX_NUM_CHARS = 256;
        int i = 0;
        char c;

        struct _mliBuff buff = _mliBuff_init();

        while (1) {
                mli_check(i <= MAX_NUM_CHARS, "Expected less chars in line.");
                c = line[i];

                /* printf("i: %d, c:%c state: %d \n", i, c, state); */

                if (state == FI) {
                        goto complete;
                }

                if (state == -1) {
                        *line_mode = -1;
                        mli_sentinel("Can not parse line.");
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
                        state = -1;
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

                mli_c(_mli_fill_buff(c, &buff, 3, state, old_state, &faces_vertices->a));
                mli_c(_mli_fill_buff(c, &buff, 5, state, old_state, &faces_vertices->b));
                mli_c(_mli_fill_buff(c, &buff, 7, state, old_state, &faces_vertices->c));

                /* mode MLI_WAVEFRONT_FACE_LINE_V_VN */

                /*mli_c(_mli_fill_buff(c, &buff, 3, state, old_state, &faces_vertices->a));*/
                mli_c(_mli_fill_buff(c, &buff, 27, state, old_state, &faces_vertex_normals->a));

                mli_c(_mli_fill_buff(c, &buff, 29, state, old_state, &faces_vertices->b));
                mli_c(_mli_fill_buff(c, &buff, 32, state, old_state, &faces_vertex_normals->b));

                mli_c(_mli_fill_buff(c, &buff, 34, state, old_state, &faces_vertices->c));
                mli_c(_mli_fill_buff(c, &buff, 37, state, old_state, &faces_vertex_normals->c));

                /* mode MLI_WAVEFRONT_FACE_LINE_V_VT_VN */

                /*mli_c(_mli_fill_buff(c, &buff, 3, state, old_state, &faces_vertices->a));*/
                mli_c(_mli_fill_buff(c, &buff, 11, state, old_state, &faces_texture_points->a));
                mli_c(_mli_fill_buff(c, &buff, 13, state, old_state, &faces_vertex_normals->a));

                mli_c(_mli_fill_buff(c, &buff, 15, state, old_state, &faces_vertices->b));
                mli_c(_mli_fill_buff(c, &buff, 17, state, old_state, &faces_texture_points->b));
                mli_c(_mli_fill_buff(c, &buff, 19, state, old_state, &faces_vertex_normals->b));

                mli_c(_mli_fill_buff(c, &buff, 21, state, old_state, &faces_vertices->c));
                mli_c(_mli_fill_buff(c, &buff, 23, state, old_state, &faces_texture_points->c));
                mli_c(_mli_fill_buff(c, &buff, 25, state, old_state, &faces_vertex_normals->c));


                old_state = state;
                i ++;
        }
complete:
        return 1;
error:
        return 0;
}