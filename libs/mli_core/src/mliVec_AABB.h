/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIVEC_AABB_H_
#define MLIVEC_AABB_H_

#include "mliVec.h"

int mliVec_overlap_aabb(
        const struct mliVec a,
        const struct mliVec aabb_lower,
        const struct mliVec aabb_upper);
#endif
