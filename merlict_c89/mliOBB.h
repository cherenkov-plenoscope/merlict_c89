/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIOBB_H_
#define MERLICT_C89_MLIOBB_H_

#include <assert.h>

#include "mliVec.h"
#include "mli_math.h"
#include "mliRay.h"
#include "mliEdge.h"

struct mliOBB {
        /*
         * Rectangular Oriented-Bounding-Box
         * oriented w.r.t. the unit-vectors.
         */
        struct mliVec lower;
        struct mliVec upper;
};

struct mliOBB mliOBB_set(const struct mliVec lower, const struct mliVec upper);
struct mliEdge mliOBB_edge(const struct mliOBB obb, const uint64_t edge_idx);
struct mliVec mliOBB_center(const struct mliOBB a);
struct mliOBB mliOBB_outermost(const struct mliOBB a, const struct mliOBB b);
int mliOBB_valid(const struct mliOBB obb);
int mliOBB_equal(const struct mliOBB a, const struct mliOBB b);
#endif
