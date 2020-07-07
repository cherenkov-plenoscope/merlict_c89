/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliDisc_OBB.h"

int mliDisc_has_overlap_obb(
        const struct mliDisc disc,
        const struct mliHomTraComp local2root_comp,
        const struct mliOBB obb)
{
        struct mliHexagon hex;
        hex.inner_radius = disc.radius;
        return mliHexagon_has_overlap_obb(hex, local2root_comp, obb);
}

struct mliOBB mliDisc_obb(
        const struct mliDisc disc,
        const struct mliHomTraComp local2root_comp)
{
        struct mliHexagon hex;
        hex.inner_radius = disc.radius;
        return mliHexagon_obb(hex, local2root_comp);
}
