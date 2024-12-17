/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_GEOMETRY_VALID_H_
#define MLI_GEOMETRY_VALID_H_

#include "geometry.h"

int mliGeometry_valid(const struct mliGeometry *geometry);
int mliGeometry_valid_objects(const struct mliGeometry *geometry);
int mliGeometry_valid_robjects_HomTras(const struct mliGeometry *geometry);
int mliGeometry_valid_object_references(const struct mliGeometry *geometry);
#endif
