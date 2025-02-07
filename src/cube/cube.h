/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_CUBE_H_
#define MLI_CUBE_H_

#include <stdint.h>
#include "../bool/bool.h"
#include "../vec/vec.h"
#include "../aabb/aabb.h"

struct mli_Cube {
        /*
         * Cubic Oriented-Bounding-Box
         * oriented w.r.t. the unit-vectors.
         */
        struct mli_Vec lower;
        double edge_length;
};

mli_bool mli_Cube_equal(const struct mli_Cube a, const struct mli_Cube b);
struct mli_Cube mli_Cube_octree_child_code(
        const struct mli_Cube cube,
        const uint8_t a);
struct mli_Cube mli_Cube_octree_child(
        const struct mli_Cube cube,
        const uint32_t sx,
        const uint32_t sy,
        const uint32_t sz);
struct mli_Cube mli_Cube_outermost_cube(const struct mli_AABB a);
struct mli_Vec mli_Cube_center(const struct mli_Cube a);
struct mli_AABB mli_Cube_to_aabb(const struct mli_Cube a);
struct mli_Vec mli_Cube_upper(const struct mli_Cube a);
#endif
