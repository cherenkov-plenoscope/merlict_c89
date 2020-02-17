/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_DISC_OBB_H_
#define MERLICT_DISC_OBB_H_

#include <math.h>
#include "mliOBB.h"
#include "mliDisc.h"
#include "mliHexagon.h"
#include "mliHexagon_OBB.h"


int mliDisc_has_overlap_obb(
    const mliDisc disc,
    const struct mliHomTraComp local2root_comp,
    const struct mliOBB obb) {
    mliHexagon hex;
    hex.inner_radius = disc.radius;
    return mliHexagon_has_overlap_obb(hex, local2root_comp, obb);}

struct mliOBB mliDisc_obb(
    const mliDisc disc,
    const struct mliHomTraComp local2root_comp) {
    mliHexagon hex;
    hex.inner_radius = disc.radius;
    return mliHexagon_obb(hex, local2root_comp);
}

#endif
