/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLISPHERE_OBB_H_
#define MERLICT_MLISPHERE_OBB_H_

#include <math.h>

mliOBB mliSphere_obb(
    const float radius,
    const mliVec translation) {
    mliOBB obb;
    mliVec r = mliVec_set(radius, radius, radius);
    obb.lower = mliVec_substract(translation, r);
    obb.upper = mliVec_add(translation, r);
    return obb;}

int mliSphere_has_overlap_obb(
    const float radius,
    const mliVec translation,
    const mliOBB obb) {
    /* Ben Voigt
     * https://stackoverflow.com/questions/4578967/cube-sphere-intersection-test
     */
    float dist_squared = radius*radius;
    if (translation.x < obb.lower.x)
        dist_squared -= MLI_SQUARED(translation.x - obb.lower.x);
    else if (translation.x > obb.upper.x)
        dist_squared -= MLI_SQUARED(translation.x - obb.upper.x);
    if (translation.y < obb.lower.y)
        dist_squared -= MLI_SQUARED(translation.y - obb.lower.y);
    else if (translation.y > obb.upper.y)
        dist_squared -= MLI_SQUARED(translation.y - obb.upper.y);
    if (translation.z < obb.lower.z)
        dist_squared -= MLI_SQUARED(translation.z - obb.lower.z);
    else if (translation.z > obb.upper.z)
        dist_squared -= MLI_SQUARED(translation.z - obb.upper.z);
    return dist_squared > 0;}

#endif
