/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "cube.h"
#include "../math/math.h"

struct mli_Vec mli_Cube_upper(const struct mli_Cube a)
{
        return mli_Vec_add(
                a.lower,
                mli_Vec_init(a.edge_length, a.edge_length, a.edge_length));
}

struct mli_AABB mli_Cube_to_aabb(const struct mli_Cube a)
{
        struct mli_AABB out;
        out.lower = a.lower;
        out.upper = mli_Cube_upper(a);
        return out;
}

struct mli_Vec mli_Cube_center(const struct mli_Cube a)
{
        return mli_Vec_init(
                a.lower.x + a.edge_length * .5,
                a.lower.y + a.edge_length * .5,
                a.lower.z + a.edge_length * .5);
}

struct mli_Cube mli_Cube_outermost_cube(const struct mli_AABB a)
{
        struct mli_Cube cube;
        struct mli_Vec center;
        struct mli_Vec half_diagonal;
        struct mli_Vec diff;
        double max_half_length;
        center = mli_AABB_center(a);
        diff = mli_Vec_substract(a.upper, a.lower);
        max_half_length = .5 * MLI_MATH_MAX3(diff.x, diff.y, diff.z);
        half_diagonal.x = max_half_length;
        half_diagonal.y = max_half_length;
        half_diagonal.z = max_half_length;
        cube.lower = mli_Vec_substract(center, half_diagonal);
        cube.edge_length = max_half_length * 2.;
        return cube;
}

struct mli_Cube mli_Cube_octree_child(
        const struct mli_Cube cube,
        const uint32_t sx,
        const uint32_t sy,
        const uint32_t sz)
{
        struct mli_Cube child;
        struct mli_Vec length;
        struct mli_Vec center = mli_Cube_center(cube);
        length = mli_Vec_substract(center, cube.lower);
        child.lower = cube.lower;
        child.edge_length = .5 * cube.edge_length;
        if (sx) {
                child.lower.x += length.x;
        }
        if (sy) {
                child.lower.y += length.y;
        }
        if (sz) {
                child.lower.z += length.z;
        }
        return child;
}

struct mli_Cube mli_Cube_octree_child_code(
        const struct mli_Cube cube,
        const uint8_t a)
{
        struct mli_Cube child;
        struct mli_Vec length;
        struct mli_Vec center = mli_Cube_center(cube);
        length = mli_Vec_substract(center, cube.lower);
        child.lower = cube.lower;
        child.edge_length = .5 * cube.edge_length;
        if (MLI_MATH_IS_BIT(a, 2)) {
                child.lower.x += length.x;
        }
        if (MLI_MATH_IS_BIT(a, 1)) {
                child.lower.y += length.y;
        }
        if (MLI_MATH_IS_BIT(a, 0)) {
                child.lower.z += length.z;
        }
        return child;
}

int mli_Cube_equal(const struct mli_Cube a, const struct mli_Cube b)
{
        if (a.edge_length != b.edge_length)
                return 0;
        if (!mli_Vec_equal(a.lower, b.lower))
                return 0;
        return 1;
}
