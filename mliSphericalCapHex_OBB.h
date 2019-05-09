/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLISPHERICALCAPHEX_OBB_H_
#define MERLICT_MLISPHERICALCAPHEX_OBB_H_

#include <math.h>
#include "mliScenery.h"
#include "mliOBB.h"
#include "mliSphere_OBB.h"


int mliSphericalCapHex_has_overlap_obb(
    const mliSphericalCapHex cap,
    const mliHomTraComp trafo,
    const mliOBB obb) {
    /* For now, we use a bounding sphere */
    const double bounding_radius = mliSphericalCapHex_bounding_radius(cap);
    return mliSphere_has_overlap_obb(bounding_radius, trafo.trans, obb);}

mliOBB mliSphericalCapHex_obb(
    const mliSphericalCapHex cap,
    const mliHomTraComp trafo) {
    /* For now, we use a sphere to enclose the spherical cap. */
    const double bounding_radius = mliSphericalCapHex_bounding_radius(cap);
    return mliSphere_obb(bounding_radius, trafo.trans);}

#endif
