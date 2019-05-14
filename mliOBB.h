/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIOBB_H_
#define MERLICT_MLIOBB_H_

#include "mliVec.h"
#include "mliMath.h"

typedef struct {
    /*
     * Rectangular Oriented-Bounding-Box
     * oriented w.r.t. the unit-vectors.
     */
    mliVec lower;
    mliVec upper;
} mliOBB;

mliOBB mliOBB_outermost(const mliOBB a, const mliOBB b) {
    mliOBB obb;
    obb.lower.x = MLI_MIN2(a.lower.x, b.lower.x);
    obb.lower.y = MLI_MIN2(a.lower.y, b.lower.y);
    obb.lower.z = MLI_MIN2(a.lower.z, b.lower.z);
    obb.upper.x = MLI_MAX2(a.upper.x, b.upper.x);
    obb.upper.y = MLI_MAX2(a.upper.y, b.upper.y);
    obb.upper.z = MLI_MAX2(a.upper.z, b.upper.z);
    return obb;}

mliVec mliOBB_center(const mliOBB a) {
    mliVec sum = mliVec_add(a.upper, a.lower);
    return mliVec_multiply(sum, .5);}

mliOBB mliOBB_dilate(const mliOBB a, const double dilation_radius) {
    mliOBB out = a;
    out.lower.x -= dilation_radius;
    out.lower.y -= dilation_radius;
    out.lower.z -= dilation_radius;
    out.upper.x += dilation_radius;
    out.upper.y += dilation_radius;
    out.upper.z += dilation_radius;
    return out;}

#endif
