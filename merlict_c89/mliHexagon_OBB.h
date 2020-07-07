/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIHEXAGON_OBB_H_
#define MERLICT_C89_MLIHEXAGON_OBB_H_

#include <math.h>

#include "mliOBB.h"
#include "mliHexagon.h"
#include "mliVec_OBB.h"
#include "mliHexagon_intersection.h"

struct mliOBB mliHexagon_obb(
        const struct mliHexagon hex,
        const struct mliHomTraComp local2root_comp);
int mliHexagon_has_overlap_obb(
        const struct mliHexagon hex,
        const struct mliHomTraComp local2root_comp,
        const struct mliOBB obb);
#endif
