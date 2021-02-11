/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIOBJECT_VALID_H_
#define MERLICT_C89_MLIOBJECT_VALID_H_

#include "mliObject.h"

int mliObject_is_valid(const struct mliObject *obj);
int mliObject_has_valid_vertices(const struct mliObject *obj);
int mliObject_has_valid_faces(const struct mliObject *obj);
int mliObject_has_valid_normals(
        const struct mliObject *obj,
        const double epsilon);
int mliObject_has_valid_materials(const struct mliObject *obj);
#endif
