/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLI_AXIS_ALIGNED_BOX_H_
#define MERLICT_MLI_AXIS_ALIGNED_BOX_H_

#include <stdint.h>
#include "mliVec.h"

int mliVec_overlap_obb(
    const struct mliVec a,
    const struct mliVec obb_lower,
    const struct mliVec obb_upper) {
    if (a.x >= obb_lower.x && a.x <= obb_upper.x &&
        a.y >= obb_lower.y && a.y <= obb_upper.y &&
        a.z >= obb_lower.z && a.z <= obb_upper.z) {
        return 1;}
    else {
        return 0;}}

#endif
