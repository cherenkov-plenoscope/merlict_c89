/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_VEC_AABB_H_
#define MLI_VEC_AABB_H_

#include "../vec/vec.h"

int mli_Vec_overlap_aabb(
        const struct mli_Vec a,
        const struct mli_Vec aabb_lower,
        const struct mli_Vec aabb_upper);
#endif
