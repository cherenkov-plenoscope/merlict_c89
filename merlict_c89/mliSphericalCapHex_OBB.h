/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISPHERICALCAPHEX_OBB_H_
#define MERLICT_C89_MLISPHERICALCAPHEX_OBB_H_

#include <math.h>

#include "mliScenery.h"
#include "mliOBB.h"
#include "mliSphere_OBB.h"

struct mliOBB mliSphericalCapHex_obb(
        const struct mliSphericalCapHex cap,
        const struct mliHomTraComp trafo);
int mliSphericalCapHex_has_overlap_obb(
        const struct mliSphericalCapHex cap,
        const struct mliHomTraComp trafo,
        const struct mliOBB obb);
#endif
