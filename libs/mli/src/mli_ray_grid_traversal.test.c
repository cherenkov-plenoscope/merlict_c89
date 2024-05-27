/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "../../mli_testing/src/mli_testing.h"
#include "../src/mli_ray_grid_traversal.h"

CASE("ray and grid")
{
        int rc;
        struct mliAxisAlignedGrid grid;
        struct mliRay ray;
        struct mliIdx3 bin;

        grid = mliAxisAlignedGrid_set(
                mliAABB_set(
                        mliVec_init(-0.5, -0.5, -0.5),
                        mliVec_init(0.5, 0.5, 0.5)
                ),
                mliIdx3_set(3, 3, 3)
        );
        CHECK_MARGIN(grid.bin_width.x, 0.333, 0.01);
        CHECK_MARGIN(grid.bin_width.y, 0.333, 0.01);
        CHECK_MARGIN(grid.bin_width.z, 0.333, 0.01);

        ray = mliRay_set(mliVec_init(0, 0 ,2), mliVec_init(0, 0, 1));
        rc = mliAxisAlignedGrid_find_voxel_of_first_interaction(
                &grid, &ray, &bin);
        CHECK(rc == MLI_AXIS_ALIGNED_GRID_RAY_DOES_NOT_INTERSECT_GRID);

        ray = mliRay_set(mliVec_init(0, 0 ,0), mliVec_init(0, 0, 1));
        rc = mliAxisAlignedGrid_find_voxel_of_first_interaction(
                &grid, &ray, &bin);
        CHECK(rc == MLI_AXIS_ALIGNED_GRID_RAY_STARTS_INSIDE_GRID);
        CHECK(bin.x == 1);
        CHECK(bin.y == 1);
        CHECK(bin.z == 1);

        ray = mliRay_set(mliVec_init(-2, 0 ,0), mliVec_init(1, 0, 0));
        rc = mliAxisAlignedGrid_find_voxel_of_first_interaction(
                &grid, &ray, &bin);
        CHECK(rc == MLI_AXIS_ALIGNED_GRID_RAY_STARTS_OUTSIDE_GRID_BUT_INTERSECTS);
        CHECK(bin.x == 0);
        CHECK(bin.y == 1);
        CHECK(bin.z == 1);

        ray = mliRay_set(mliVec_init(2, 0 ,0), mliVec_init(-1, 0, 0));
        rc = mliAxisAlignedGrid_find_voxel_of_first_interaction(
                &grid, &ray, &bin);
        CHECK(rc == MLI_AXIS_ALIGNED_GRID_RAY_STARTS_OUTSIDE_GRID_BUT_INTERSECTS);
        CHECK(bin.x == 2);
        CHECK(bin.y == 1);
        CHECK(bin.z == 1);


        ray = mliRay_set(mliVec_init(-2, -2 ,-2), mliVec_init(-1, -1, -1));
        rc = mliAxisAlignedGrid_find_voxel_of_first_interaction(
                &grid, &ray, &bin);
        CHECK(rc == MLI_AXIS_ALIGNED_GRID_RAY_DOES_NOT_INTERSECT_GRID);


        ray = mliRay_set(mliVec_init(-2, -2 ,-2), mliVec_init(1, 1, 1));
        rc = mliAxisAlignedGrid_find_voxel_of_first_interaction(
                &grid, &ray, &bin);
        CHECK(rc == MLI_AXIS_ALIGNED_GRID_RAY_STARTS_OUTSIDE_GRID_BUT_INTERSECTS);
        CHECK(bin.x == 0);
        CHECK(bin.y == 0);
        CHECK(bin.z == 0);


        ray = mliRay_set(mliVec_init(2, 2 ,2), mliVec_init(-1, -1, -1));
        rc = mliAxisAlignedGrid_find_voxel_of_first_interaction(
                &grid, &ray, &bin);
        CHECK(rc == MLI_AXIS_ALIGNED_GRID_RAY_STARTS_OUTSIDE_GRID_BUT_INTERSECTS);
        CHECK(bin.x == 2);
        CHECK(bin.y == 2);
        CHECK(bin.z == 2);


        ray = mliRay_set(mliVec_init(0, 2 ,2), mliVec_init(0, -1, -1));
        rc = mliAxisAlignedGrid_find_voxel_of_first_interaction(
                &grid, &ray, &bin);
        CHECK(rc == MLI_AXIS_ALIGNED_GRID_RAY_STARTS_OUTSIDE_GRID_BUT_INTERSECTS);
        CHECK(bin.x == 1);
        CHECK(bin.y == 2);
        CHECK(bin.z == 2);
}

CASE("ray and grid traversal simple")
{
        int rc;
        struct mliAxisAlignedGrid grid;
        struct mliAxisAlignedGridTraversal traversal;
        struct mliRay ray;

        grid = mliAxisAlignedGrid_set(
                mliAABB_set(
                        mliVec_init(-2.5, -1.5, -1),
                        mliVec_init(2.5, 1.5, 1)
                ),
                mliIdx3_set(5, 3, 2)
        );
        CHECK_MARGIN(grid.bin_width.x, 1.0, 0.01);
        CHECK_MARGIN(grid.bin_width.y, 1.0, 0.01);
        CHECK_MARGIN(grid.bin_width.z, 1.0, 0.01);

        /* X-AXIS */
        ray = mliRay_set(mliVec_init(-3, 0.2 ,0.1), mliVec_init(1, 0, 0));
        traversal = mliAxisAlignedGridTraversal_start(&grid, &ray);
        CHECK(traversal.valid);
        CHECK(traversal.voxel.x == 0);
        CHECK(traversal.voxel.y == 1);
        CHECK(traversal.voxel.z == 1);

        rc = mliAxisAlignedGridTraversal_next(&traversal);
        CHECK(rc == traversal.valid);
        CHECK(traversal.valid);
        CHECK(traversal.voxel.x == 1);
        CHECK(traversal.voxel.y == 1);
        CHECK(traversal.voxel.z == 1);

        rc = mliAxisAlignedGridTraversal_next(&traversal);
        CHECK(rc == traversal.valid);
        CHECK(traversal.valid);
        CHECK(traversal.voxel.x == 2);
        CHECK(traversal.voxel.y == 1);
        CHECK(traversal.voxel.z == 1);

        rc = mliAxisAlignedGridTraversal_next(&traversal);
        CHECK(rc == traversal.valid);
        CHECK(traversal.valid);
        CHECK(traversal.voxel.x == 3);
        CHECK(traversal.voxel.y == 1);
        CHECK(traversal.voxel.z == 1);

        rc = mliAxisAlignedGridTraversal_next(&traversal);
        CHECK(rc == traversal.valid);
        CHECK(traversal.valid);
        CHECK(traversal.voxel.x == 4);
        CHECK(traversal.voxel.y == 1);
        CHECK(traversal.voxel.z == 1);

        rc = mliAxisAlignedGridTraversal_next(&traversal);
        CHECK(rc == traversal.valid);
        CHECK(!traversal.valid);

        /* Y-AXIS */
        ray = mliRay_set(mliVec_init(0.2, 8.3 ,0.1), mliVec_init(0, -1, 0));
        traversal = mliAxisAlignedGridTraversal_start(&grid, &ray);
        CHECK(traversal.valid);
        CHECK(traversal.voxel.x == 2);
        CHECK(traversal.voxel.y == 2);
        CHECK(traversal.voxel.z == 1);

        rc = mliAxisAlignedGridTraversal_next(&traversal);
        CHECK(rc == traversal.valid);
        CHECK(traversal.valid);
        CHECK(traversal.voxel.x == 2);
        CHECK(traversal.voxel.y == 1);
        CHECK(traversal.voxel.z == 1);

        rc = mliAxisAlignedGridTraversal_next(&traversal);
        CHECK(rc == traversal.valid);
        CHECK(traversal.valid);
        CHECK(traversal.voxel.x == 2);
        CHECK(traversal.voxel.y == 0);
        CHECK(traversal.voxel.z == 1);

        rc = mliAxisAlignedGridTraversal_next(&traversal);
        CHECK(rc == traversal.valid);
        CHECK(!traversal.valid);

        /* Z-AXIS */
        ray = mliRay_set(mliVec_init(0.2, -0.1, -1223.0), mliVec_init(0, 0, 1));
        traversal = mliAxisAlignedGridTraversal_start(&grid, &ray);
        CHECK(traversal.valid);
        CHECK(traversal.voxel.x == 2);
        CHECK(traversal.voxel.y == 1);
        CHECK(traversal.voxel.z == 0);

        rc = mliAxisAlignedGridTraversal_next(&traversal);
        CHECK(rc == traversal.valid);
        CHECK(traversal.valid);
        CHECK(traversal.voxel.x == 2);
        CHECK(traversal.voxel.y == 1);
        CHECK(traversal.voxel.z == 1);

        rc = mliAxisAlignedGridTraversal_next(&traversal);
        CHECK(rc == traversal.valid);
        CHECK(!traversal.valid);

        /* start inside */
        ray = mliRay_set(mliVec_init(0.0, 0.0, 0.5), mliVec_init(1, 0, 0));
        traversal = mliAxisAlignedGridTraversal_start(&grid, &ray);
        CHECK(traversal.valid);
        CHECK(traversal.voxel.x == 2);
        CHECK(traversal.voxel.y == 1);
        CHECK(traversal.voxel.z == 1);

        mliAxisAlignedGridTraversal_fprint(stderr, &traversal);
        rc = mliAxisAlignedGridTraversal_next(&traversal);
        mliAxisAlignedGridTraversal_fprint(stderr, &traversal);

        CHECK(rc == traversal.valid);
        CHECK(traversal.valid);
        CHECK(traversal.voxel.x == 3);
        CHECK(traversal.voxel.y == 1);
        CHECK(traversal.voxel.z == 1);

        rc = mliAxisAlignedGridTraversal_next(&traversal);
        CHECK(rc == traversal.valid);
        CHECK(traversal.valid);
        CHECK(traversal.voxel.x == 4);
        CHECK(traversal.voxel.y == 1);
        CHECK(traversal.voxel.z == 1);

        rc = mliAxisAlignedGridTraversal_next(&traversal);
        CHECK(rc == traversal.valid);
        CHECK(!traversal.valid);
}


CASE("Actual example from simulated shower")
{
        int rc = 10;
        struct mliAxisAlignedGrid grid;
        struct mliAxisAlignedGridTraversal traversal;
        struct mliRay ray;

        grid = mliAxisAlignedGrid_set(
                mliAABB_set(
                        mliVec_init(-5119268.215672, -5119974.912158, -5000.000000),
                        mliVec_init(5120731.784328, 5120025.087842, 5000.000000)
                ),
                mliIdx3_set(1024, 1024, 1)
        );

        ray.support = mliVec_init(4.171968e+03, 4.857704e+03, 0.000000e+00);
        ray.direction = mliVec_init(-3.894984e-02, -6.049007e-01, 7.953478e-01);

        traversal = mliAxisAlignedGridTraversal_start(
                &grid,
                &ray
        );
        mliAxisAlignedGridTraversal_fprint(stderr, &traversal);

        while (traversal.valid && rc > 0) {
                rc -= 1;
                mliAxisAlignedGridTraversal_next(&traversal);
                mliAxisAlignedGridTraversal_fprint(stderr, &traversal);
        }
}
