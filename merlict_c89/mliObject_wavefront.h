/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIOBJECT_WAVEFRONT_H_
#define MERLICT_C89_MLIOBJECT_WAVEFRONT_H_

#include <stdint.h>
#include "mliObject.h"

int _mliObject_parse_face_line(
        const char *line,
        struct mliFace *faces_vertices,
        struct mliFace *faces_texture_points,
        struct mliFace *faces_vertex_normals,
        int *line_mode);
int _mliObject_parse_three_float_line(const char *line, struct mliVec *v);

int mliObject_malloc_from_string(struct mliObject *obj, const char *str);

int64_t mliObject_fprint(FILE *f, const struct mliObject *obj);
#endif
