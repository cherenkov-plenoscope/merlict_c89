/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliVec_OBB.h"

/* Copyright 2019 Sebastian Achim Mueller */

int mliVec_overlap_obb(
        const struct mliVec a,
        const struct mliVec obb_lower,
        const struct mliVec obb_upper)
{
        if (a.x >= obb_lower.x && a.x <= obb_upper.x && a.y >= obb_lower.y &&
            a.y <= obb_upper.y && a.z >= obb_lower.z && a.z <= obb_upper.z) {
                return 1;
        } else {
                return 0;
        }
}
