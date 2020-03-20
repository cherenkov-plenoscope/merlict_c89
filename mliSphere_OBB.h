/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLISPHERE_OBB_H_
#define MERLICT_MLISPHERE_OBB_H_

#include <math.h>
#include "mli_math.h"

struct mliOBB mliSphere_obb(
    const double radius,
    const struct mliVec translation) {
    struct mliOBB obb;
    struct mliVec r = mliVec_set(radius, radius, radius);
    obb.lower = mliVec_substract(translation, r);
    obb.upper = mliVec_add(translation, r);
    return obb;}

int mliSphere_has_overlap_obb(
    const double radius,
    const struct mliVec translation,
    const struct mliOBB obb) {
    /*
     * Ben Voigt
     * https://stackoverflow.com/questions/4578967/cube-sphere-intersection-test
     */
    double dist_squared = radius*radius;
    if (translation.x < obb.lower.x)
        dist_squared -= mli_squared(translation.x - obb.lower.x);
    else if (translation.x > obb.upper.x)
        dist_squared -= mli_squared(translation.x - obb.upper.x);
    if (translation.y < obb.lower.y)
        dist_squared -= mli_squared(translation.y - obb.lower.y);
    else if (translation.y > obb.upper.y)
        dist_squared -= mli_squared(translation.y - obb.upper.y);
    if (translation.z < obb.lower.z)
        dist_squared -= mli_squared(translation.z - obb.lower.z);
    else if (translation.z > obb.upper.z)
        dist_squared -= mli_squared(translation.z - obb.upper.z);
    return dist_squared > 0;}

#endif
