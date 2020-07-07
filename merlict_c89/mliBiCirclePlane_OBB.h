/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIBICIRCLEPLANE_OBB_H_
#define MERLICT_C89_MLIBICIRCLEPLANE_OBB_H_

#include <math.h>
#include <assert.h>

#include "mliOBB.h"
#include "mliBiCirclePlane.h"
#include "mliTriangle_OBB.h"

struct mliOBB mliBiCirclePlane_obb(
        const struct mliBiCirclePlane plane,
        const struct mliHomTraComp local2root_comp);
int mliBiCirclePlane_has_overlap_obb(
        const struct mliBiCirclePlane plane,
        const struct mliHomTraComp local2root_comp,
        const struct mliOBB obb);
struct mliVec __mliBiCirclePlane_rectangle_corners(
        const struct mliBiCirclePlane plane,
        const uint64_t corner_idx);
#endif
