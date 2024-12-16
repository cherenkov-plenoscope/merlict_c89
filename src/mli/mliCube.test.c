/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mliCube_octree_child")
{
        struct mliCube node;
        struct mliCube child;
        node.lower = mli_Vec_init(.0, .0, .0);
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
        struct mliAABB a;
        struct mliCube cube;
        cube.lower = mli_Vec_init(.0, .0, .0);
        cube.edge_length = 1.;
        a = mliCube_to_aabb(cube);
        CHECK(mli_Vec_equal_margin(a.lower, cube.lower, 1e-6));
        CHECK(mli_Vec_equal_margin(a.upper, mli_Vec_init(1., 1., 1.), 1e-6));
}

CASE("mliCube_outermost_cube")
{
        struct mliAABB a;
        struct mliCube cube;
        a.lower = mli_Vec_init(.0, .0, .0);
        a.upper = mli_Vec_init(1., 2., 3.);
        cube = mliCube_outermost_cube(a);

        CHECK_MARGIN(cube.lower.x, 0.5 - 1.5, 1e-7);
        CHECK_MARGIN(cube.lower.y, 1. - 1.5, 1e-7);
        CHECK_MARGIN(cube.lower.z, 1.5 - 1.5, 1e-7);
        CHECK_MARGIN(cube.edge_length, 3., 1e-7);
}

CASE("mliAABB_center")
{
        struct mli_Vec center;
        struct mliAABB aabb;
        aabb.lower = mli_Vec_init(.5, .5, .5);
        aabb.upper = mli_Vec_init(1.5, 1.5, 1.5);
        center = mliAABB_center(aabb);
        CHECK_MARGIN(center.x, 1., 1e-7);
        CHECK_MARGIN(center.y, 1., 1e-7);
        CHECK_MARGIN(center.z, 1., 1e-7);
}

CASE("mliTriangle_aabb")
{
        struct mliAABB aabb = mliTriangle_aabb(
                mli_Vec_init(-5., 2., -.8),
                mli_Vec_init(20., -3., 19.),
                mli_Vec_init(10., 6., 2.5));
        CHECK_MARGIN(aabb.lower.x, -5., 1e-7);
        CHECK_MARGIN(aabb.lower.y, -3., 1e-7);
        CHECK_MARGIN(aabb.lower.z, -.8, 1e-7);
        CHECK_MARGIN(aabb.upper.x, 20., 1e-7);
        CHECK_MARGIN(aabb.upper.y, 6., 1e-7);
        CHECK_MARGIN(aabb.upper.z, 19., 1e-7);
}

CASE("mli_Vec_overlap_aabb")
{
        struct mli_Vec point_inside = {1., 1., 1.};
        struct mli_Vec point_outside = {-1., -2., -3.};
        struct mli_Vec low = {0., 0., 0.};
        struct mli_Vec upp = {2., 2., 2.};
        CHECK(mli_Vec_overlap_aabb(point_inside, low, upp));
        CHECK(!mli_Vec_overlap_aabb(point_outside, low, upp));
}

CASE("mli_Vec_overlap_aabb, upper, and lower cases")
{
        struct mli_Vec low = {0., 0., 0.};
        struct mli_Vec upp = {2., 2., 2.};
        CHECK(mli_Vec_overlap_aabb(mli_Vec_init(0., 0., 0.), low, upp));
        CHECK(mli_Vec_overlap_aabb(mli_Vec_init(0., 0., 1.), low, upp));
        CHECK(mli_Vec_overlap_aabb(mli_Vec_init(0., 1., 1.), low, upp));
        CHECK(mli_Vec_overlap_aabb(mli_Vec_init(2., 2., 2.), low, upp));
        CHECK(mli_Vec_overlap_aabb(mli_Vec_init(1., 2., 2.), low, upp));
        CHECK(mli_Vec_overlap_aabb(mli_Vec_init(1., 1., 2.), low, upp));
}

CASE("mliTriangle_has_overlap_aabb")
{
        struct mliAABB aabb;
        aabb.lower = mli_Vec_init(0., 0., 0.);
        aabb.upper = mli_Vec_init(2., 2., 2.);
        CHECK(mliTriangle_has_overlap_aabb(
                mli_Vec_init(0., 0., 0.),
                mli_Vec_init(2., 2., 2.),
                mli_Vec_init(1., 1., 1.),
                aabb));

        CHECK(!mliTriangle_has_overlap_aabb(
                mli_Vec_init(0., 0., 5.),
                mli_Vec_init(0., 1., 5.),
                mli_Vec_init(1., 1., 5.),
                aabb));

        CHECK(mliTriangle_has_overlap_aabb(
                mli_Vec_init(0., 0., 2.),
                mli_Vec_init(0., 1., 2.),
                mli_Vec_init(1., 1., 2.),
                aabb));

        CHECK(mliTriangle_has_overlap_aabb(
                mli_Vec_init(2., 2., 1.),
                mli_Vec_init(2., 3., 1.),
                mli_Vec_init(3., 3., 1.),
                aabb));

        CHECK(!mliTriangle_has_overlap_aabb(
                mli_Vec_init(2.1, 2., 1.),
                mli_Vec_init(2.1, 3., 1.),
                mli_Vec_init(3., 3., 1.),
                aabb));

        CHECK(mliTriangle_has_overlap_aabb(
                mli_Vec_init(-50, -50, 1.),
                mli_Vec_init(50, -50, 1.),
                mli_Vec_init(0., 50, 1.),
                aabb));
}
