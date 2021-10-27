/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIGEOMETRY_EQUAL_H_
#define MLIGEOMETRY_EQUAL_H_

#include "mliGeometry.h"

int mliGeometry_equal(const struct mliGeometry *a, const struct mliGeometry *b);
int mliGeometry_objects_equal(
        const struct mliGeometry *a,
        const struct mliGeometry *b);
int mliGeometry_object_references_equal(
        const struct mliGeometry *a,
        const struct mliGeometry *b);
#endif
