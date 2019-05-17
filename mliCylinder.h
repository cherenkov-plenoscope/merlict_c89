/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLICYLINDER_H_
#define MERLICT_MLICYLINDER_H_

#include <math.h>
#include "mliMath.h"

typedef struct {
    double radius;
    double length;
} mliCylinder;

int mliCylinder_is_equal(
    const mliCylinder a,
    const mliCylinder b) {
    if (a.radius != b.radius) return 0;
    if (a.length != b.length) return 0;
    return 1;}

#endif
