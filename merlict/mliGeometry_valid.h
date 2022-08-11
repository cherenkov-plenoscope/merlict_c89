/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIGEOMETRY_VALID_H_
#define MLIGEOMETRY_VALID_H_

#include "mliGeometry.h"

int mliGeometry_valid(const struct mliGeometry *geometry);
int mliGeometry_valid_objects(const struct mliGeometry *geometry);
int mliGeometry_valid_robjects_HomTras(const struct mliGeometry *geometry);
int mliGeometry_valid_object_references(const struct mliGeometry *geometry);
#endif
