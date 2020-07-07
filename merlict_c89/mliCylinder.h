/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLICYLINDER_H_
#define MERLICT_C89_MLICYLINDER_H_

#include <math.h>

struct mliCylinder {
        double radius;
        double length;
};

int mliCylinder_is_equal(
        const struct mliCylinder a,
        const struct mliCylinder b);
#endif
