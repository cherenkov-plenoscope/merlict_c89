/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLISPHERICALCAPHEX_OBB_H_
#define MERLICT_MLISPHERICALCAPHEX_OBB_H_

#include <math.h>
#include "mliScenery.h"
#include "mliOBB.h"


int mliSphericalCapHex_has_overlap_obb(
    const mliSphericalCapHeagonal cap,
    const mliHomTraComp trafo,
    const mliOBB obb) {
    /* FIXME */
    /* assume spherical */
    /* float bounding_radius = mliSphericalCapHex_bounding_radius(cap);*/

    return 1;}

mliOBB mliSphericalCapHex_obb(
    const mliSphericalCapHeagonal cap,
    const mliHomTraComp trafo) {
    mliOBB obb;
    /* FIXME */
    obb.lower = mliVec_set(0., 0., 0);
    obb.upper = mliVec_set(0., 0., 0);
    return obb;}

#endif
