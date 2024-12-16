/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "vec_AABB.h"

int mli_Vec_overlap_aabb(
        const struct mli_Vec a,
        const struct mli_Vec aabb_lower,
        const struct mli_Vec aabb_upper)
{
        if (a.x >= aabb_lower.x && a.x <= aabb_upper.x && a.y >= aabb_lower.y &&
            a.y <= aabb_upper.y && a.z >= aabb_lower.z && a.z <= aabb_upper.z) {
                return 1;
        } else {
                return 0;
        }
}
