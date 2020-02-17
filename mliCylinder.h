/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLICYLINDER_H_
#define MERLICT_MLICYLINDER_H_

#include <math.h>

struct mliCylinder {
    double radius;
    double length;
};

int mliCylinder_is_equal(
    const struct mliCylinder a,
    const struct mliCylinder b)
{
    if (a.radius != b.radius) return 0;
    if (a.length != b.length) return 0;
    return 1;
}

#endif
