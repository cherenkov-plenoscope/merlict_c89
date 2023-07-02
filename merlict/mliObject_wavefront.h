/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIOBJECT_WAVEFRONT_H_
#define MLIOBJECT_WAVEFRONT_H_

#include <stdint.h>
#include "mliObject.h"
#include "mliIo.h"
#include "mliStr.h"

int mliObject_malloc_from_wavefront(struct mliObject *obj, const char *str);
int mliObject_fprint_to_wavefront(struct mliIo *f, const struct mliObject *obj);
int mliObject_parse_face_line(
        const char *line,
        struct mliFace *faces_vertices,
        struct mliFace *faces_texture_points,
        struct mliFace *faces_vertex_normals,
        int *line_mode);
int mliObject_parse_three_float_line(const char *line, struct mliVec *v);
#endif
