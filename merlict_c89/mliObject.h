/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_WAVEFRONT_OBJECT_H_
#define MERLICT_C89_MLI_WAVEFRONT_OBJECT_H_

#include <stdint.h>
#include "mliVec.h"
#include "mliFace.h"

struct mliObject {
        uint32_t num_vertices;
        struct mliVec *vertices;

        uint32_t num_vertex_normals;
        struct mliVec *vertex_normals;

        uint32_t num_faces;
        struct mliFace *faces_vertices;
        struct mliFace *faces_vertex_normals;
};

int mliObject_malloc(
        struct mliObject *obj,
        const uint64_t num_vertices,
        const uint64_t num_vertex_normals,
        const uint64_t num_faces);
void mliObject_free(struct mliObject *obj);
struct mliObject mliObject_init(void);
int mliObject_assert_valid_faces(const struct mliObject *obj);

int _mliObject_parse_face_line(
        const char *line,
        struct mliFace *faces_vertices,
        struct mliFace *faces_texture_points,
        struct mliFace *faces_vertex_normals,
        int *line_mode);
int _mliObject_parse_three_float_line(
        const char *line,
        struct mliVec *v);
#endif
