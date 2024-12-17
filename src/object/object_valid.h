/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_OBJECT_VALID_H_
#define MLI_OBJECT_VALID_H_

#include <stdint.h>
#include "object.h"

int mliObject_is_valid(const struct mliObject *obj);
int mliObject_has_valid_vertices(const struct mliObject *obj);
int mliObject_has_valid_faces(const struct mliObject *obj);
int mliObject_has_valid_normals(
        const struct mliObject *obj,
        const double epsilon);
int mliObject_has_valid_materials(const struct mliObject *obj);
int mliObject_num_unused(
        const struct mliObject *obj,
        uint32_t *num_unused_vertices,
        uint32_t *num_unused_vertex_normals,
        uint32_t *num_unused_materials);
#endif
