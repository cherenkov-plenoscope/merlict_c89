/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "mli_testing.h"
#include "mliCube.h"

CASE("mliCube_octree_child")
{
        struct mliCube node;
        struct mliCube child;
        node.lower = mliVec_init(.0, .0, .0);
        node.edge_length = 2.;
        child = mliCube_octree_child(node, 0, 0, 0);
        CHECK_MARGIN(child.lower.x, 0., 1e-7);
        CHECK_MARGIN(child.lower.y, 0., 1e-7);
        CHECK_MARGIN(child.lower.z, 0., 1e-7);
        CHECK_MARGIN(child.edge_length, 1., 1e-7);

        child = mliCube_octree_child(node, 0, 0, 1);
        CHECK_MARGIN(child.lower.x, 0., 1e-7);
        CHECK_MARGIN(child.lower.y, 0., 1e-7);
        CHECK_MARGIN(child.lower.z, 1., 1e-7);
        CHECK_MARGIN(child.edge_length, 1., 1e-7);

        child = mliCube_octree_child(node, 0, 1, 0);
        CHECK_MARGIN(child.lower.x, 0., 1e-7);
        CHECK_MARGIN(child.lower.y, 1., 1e-7);
        CHECK_MARGIN(child.lower.z, 0., 1e-7);
        CHECK_MARGIN(child.edge_length, 1., 1e-7);

        child = mliCube_octree_child(node, 1, 0, 0);
        CHECK_MARGIN(child.lower.x, 1., 1e-7);
        CHECK_MARGIN(child.lower.y, 0., 1e-7);
        CHECK_MARGIN(child.lower.z, 0., 1e-7);
        CHECK_MARGIN(child.edge_length, 1., 1e-7);
}

CASE("Cube to Orientated-Bounding-Box")
{
        struct mliOBB a;
        struct mliCube cube;
        cube.lower = mliVec_init(.0, .0, .0);
        cube.edge_length = 1.;
        a = mliCube_to_obb(cube);
        CHECK(mliVec_equal_margin(a.lower, cube.lower, 1e-6));
        CHECK(mliVec_equal_margin(a.upper, mliVec_init(1., 1., 1.), 1e-6));
}

CASE("mliCube_outermost_cube")
{
        struct mliOBB a;
        struct mliCube cube;
        a.lower = mliVec_init(.0, .0, .0);
        a.upper = mliVec_init(1., 2., 3.);
        cube = mliCube_outermost_cube(a);

        CHECK_MARGIN(cube.lower.x, 0.5 - 1.5, 1e-7);
        CHECK_MARGIN(cube.lower.y, 1. - 1.5, 1e-7);
        CHECK_MARGIN(cube.lower.z, 1.5 - 1.5, 1e-7);
        CHECK_MARGIN(cube.edge_length, 3., 1e-7);
}

CASE("mliOBB_center")
{
        struct mliVec center;
        struct mliOBB obb;
        obb.lower = mliVec_init(.5, .5, .5);
        obb.upper = mliVec_init(1.5, 1.5, 1.5);
        center = mliOBB_center(obb);
        CHECK_MARGIN(center.x, 1., 1e-7);
        CHECK_MARGIN(center.y, 1., 1e-7);
        CHECK_MARGIN(center.z, 1., 1e-7);
}

CASE("mliTriangle_obb")
{
        struct mliOBB obb = mliTriangle_obb(
                mliVec_init(-5., 2., -.8),
                mliVec_init(20., -3., 19.),
                mliVec_init(10., 6., 2.5));
        CHECK_MARGIN(obb.lower.x, -5., 1e-7);
        CHECK_MARGIN(obb.lower.y, -3., 1e-7);
        CHECK_MARGIN(obb.lower.z, -.8, 1e-7);
        CHECK_MARGIN(obb.upper.x, 20., 1e-7);
        CHECK_MARGIN(obb.upper.y, 6., 1e-7);
        CHECK_MARGIN(obb.upper.z, 19., 1e-7);
}

CASE("mliVec_overlap_obb")
{
        struct mliVec point_inside = {1., 1., 1.};
        struct mliVec point_outside = {-1., -2., -3.};
        struct mliVec low = {0., 0., 0.};
        struct mliVec upp = {2., 2., 2.};
        CHECK(mliVec_overlap_obb(point_inside, low, upp));
        CHECK(!mliVec_overlap_obb(point_outside, low, upp));
}

CASE("mliVec_overlap_obb, upper, and lower cases")
{
        struct mliVec low = {0., 0., 0.};
        struct mliVec upp = {2., 2., 2.};
        CHECK(mliVec_overlap_obb(mliVec_init(0., 0., 0.), low, upp));
        CHECK(mliVec_overlap_obb(mliVec_init(0., 0., 1.), low, upp));
        CHECK(mliVec_overlap_obb(mliVec_init(0., 1., 1.), low, upp));
        CHECK(mliVec_overlap_obb(mliVec_init(2., 2., 2.), low, upp));
        CHECK(mliVec_overlap_obb(mliVec_init(1., 2., 2.), low, upp));
        CHECK(mliVec_overlap_obb(mliVec_init(1., 1., 2.), low, upp));
}

CASE("mliTriangle_has_overlap_obb")
{
        struct mliOBB obb;
        obb.lower = mliVec_init(0., 0., 0.);
        obb.upper = mliVec_init(2., 2., 2.);
        CHECK(mliTriangle_has_overlap_obb(
                mliVec_init(0., 0., 0.),
                mliVec_init(2., 2., 2.),
                mliVec_init(1., 1., 1.),
                obb));

        CHECK(!mliTriangle_has_overlap_obb(
                mliVec_init(0., 0., 5.),
                mliVec_init(0., 1., 5.),
                mliVec_init(1., 1., 5.),
                obb));

        CHECK(mliTriangle_has_overlap_obb(
                mliVec_init(0., 0., 2.),
                mliVec_init(0., 1., 2.),
                mliVec_init(1., 1., 2.),
                obb));

        CHECK(mliTriangle_has_overlap_obb(
                mliVec_init(2., 2., 1.),
                mliVec_init(2., 3., 1.),
                mliVec_init(3., 3., 1.),
                obb));

        CHECK(!mliTriangle_has_overlap_obb(
                mliVec_init(2.1, 2., 1.),
                mliVec_init(2.1, 3., 1.),
                mliVec_init(3., 3., 1.),
                obb));

        CHECK(mliTriangle_has_overlap_obb(
                mliVec_init(-50, -50, 1.),
                mliVec_init(50, -50, 1.),
                mliVec_init(0., 50, 1.),
                obb));
}
