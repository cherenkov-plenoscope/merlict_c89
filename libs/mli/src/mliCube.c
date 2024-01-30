/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliCube.h"
#include "mli_math.h"

struct mliVec mliCube_upper(const struct mliCube a)
{
        return mliVec_add(
                a.lower,
                mliVec_init(a.edge_length, a.edge_length, a.edge_length));
}

struct mliAABB mliCube_to_aabb(const struct mliCube a)
{
        struct mliAABB out;
        out.lower = a.lower;
        out.upper = mliCube_upper(a);
        return out;
}

struct mliVec mliCube_center(const struct mliCube a)
{
        return mliVec_init(
                a.lower.x + a.edge_length * .5,
                a.lower.y + a.edge_length * .5,
                a.lower.z + a.edge_length * .5);
}

struct mliCube mliCube_outermost_cube(const struct mliAABB a)
{
        struct mliCube cube;
        struct mliVec center;
        struct mliVec half_diagonal;
        struct mliVec diff;
        double max_half_length;
        center = mliAABB_center(a);
        diff = mliVec_substract(a.upper, a.lower);
        max_half_length = .5 * MLI_MAX3(diff.x, diff.y, diff.z);
        half_diagonal.x = max_half_length;
        half_diagonal.y = max_half_length;
        half_diagonal.z = max_half_length;
        cube.lower = mliVec_substract(center, half_diagonal);
        cube.edge_length = max_half_length * 2.;
        return cube;
}

struct mliCube mliCube_octree_child(
        const struct mliCube cube,
        const uint32_t sx,
        const uint32_t sy,
        const uint32_t sz)
{
        struct mliCube child;
        struct mliVec length;
        struct mliVec center = mliCube_center(cube);
        length = mliVec_substract(center, cube.lower);
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

struct mliCube mliCube_octree_child_code(
        const struct mliCube cube,
        const uint8_t a)
{
        struct mliCube child;
        struct mliVec length;
        struct mliVec center = mliCube_center(cube);
        length = mliVec_substract(center, cube.lower);
        child.lower = cube.lower;
        child.edge_length = .5 * cube.edge_length;
        if (MLI_IS_BIT(a, 2)) {
                child.lower.x += length.x;
        }
        if (MLI_IS_BIT(a, 1)) {
                child.lower.y += length.y;
        }
        if (MLI_IS_BIT(a, 0)) {
                child.lower.z += length.z;
        }
        return child;
}

int mliCube_equal(const struct mliCube a, const struct mliCube b)
{
        if (a.edge_length != b.edge_length)
                return 0;
        if (!mliVec_equal(a.lower, b.lower))
                return 0;
        return 1;
}
