/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIDISC_OBB_H_
#define MERLICT_C89_MLIDISC_OBB_H_

#include <math.h>

#include "mliOBB.h"
#include "mliDisc.h"
#include "mliHexagon.h"
#include "mliHexagon_OBB.h"

struct mliOBB mliDisc_obb(
        const struct mliDisc disc,
        const struct mliHomTraComp local2root_comp);
int mliDisc_has_overlap_obb(
        const struct mliDisc disc,
        const struct mliHomTraComp local2root_comp,
        const struct mliOBB obb);
#endif
