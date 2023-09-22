/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLICUBE_H_
#define MLICUBE_H_

#include <stdint.h>
#include "mliVec.h"
#include "mliAABB.h"

#define MLI_IS_BIT(var, pos) ((var) & (1 << (pos)))

struct mliCube {
        /*
         * Cubic Oriented-Bounding-Box
         * oriented w.r.t. the unit-vectors.
         */
        struct mliVec lower;
        double edge_length;
};

int mliCube_equal(const struct mliCube a, const struct mliCube b);
struct mliCube mliCube_octree_child_code(
        const struct mliCube cube,
        const uint8_t a);
struct mliCube mliCube_octree_child(
        const struct mliCube cube,
        const uint32_t sx,
        const uint32_t sy,
        const uint32_t sz);
struct mliCube mliCube_outermost_cube(const struct mliAABB a);
struct mliVec mliCube_center(const struct mliCube a);
struct mliAABB mliCube_to_aabb(const struct mliCube a);
struct mliVec mliCube_upper(const struct mliCube a);
#endif
