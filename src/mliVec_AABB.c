/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliVec_AABB.h"

int mliVec_overlap_aabb(
        const struct mliVec a,
        const struct mliVec aabb_lower,
        const struct mliVec aabb_upper)
{
        if (a.x >= aabb_lower.x && a.x <= aabb_upper.x && a.y >= aabb_lower.y &&
            a.y <= aabb_upper.y && a.z >= aabb_lower.z && a.z <= aabb_upper.z) {
                return 1;
        } else {
                return 0;
        }
}
