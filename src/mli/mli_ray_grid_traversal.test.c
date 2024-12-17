/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("ray and grid")
{
        int rc;
        struct mliAxisAlignedGrid grid;
        struct mliRay ray;
        struct mliIdx3 bin;

        grid = mliAxisAlignedGrid_set(
                mli_AABB_set(
                        mli_Vec_init(-0.5, -0.5, -0.5),
                        mli_Vec_init(0.5, 0.5, 0.5)),
                mliIdx3_set(3, 3, 3));
        CHECK_MARGIN(grid.bin_width.x, 0.333, 0.01);
        CHECK_MARGIN(grid.bin_width.y, 0.333, 0.01);
        CHECK_MARGIN(grid.bin_width.z, 0.333, 0.01);

        ray = mliRay_set(mli_Vec_init(0, 0, 2), mli_Vec_init(0, 0, 1));
        rc = mliAxisAlignedGrid_find_voxel_of_first_interaction(
                &grid, &ray, &bin);
        CHECK(rc == MLI_AXIS_ALIGNED_GRID_RAY_DOES_NOT_INTERSECT_GRID);

        ray = mliRay_set(mli_Vec_init(0, 0, 0), mli_Vec_init(0, 0, 1));
        rc = mliAxisAlignedGrid_find_voxel_of_first_interaction(
                &grid, &ray, &bin);
        CHECK(rc == MLI_AXIS_ALIGNED_GRID_RAY_STARTS_INSIDE_GRID);
        CHECK(bin.x == 1);
        CHECK(bin.y == 1);
        CHECK(bin.z == 1);

        ray = mliRay_set(mli_Vec_init(-2, 0, 0), mli_Vec_init(1, 0, 0));
        rc = mliAxisAlignedGrid_find_voxel_of_first_interaction(
                &grid, &ray, &bin);
        CHECK(rc ==
              MLI_AXIS_ALIGNED_GRID_RAY_STARTS_OUTSIDE_GRID_BUT_INTERSECTS);
        CHECK(bin.x == 0);
        CHECK(bin.y == 1);
        CHECK(bin.z == 1);

        ray = mliRay_set(mli_Vec_init(2, 0, 0), mli_Vec_init(-1, 0, 0));
        rc = mliAxisAlignedGrid_find_voxel_of_first_interaction(
                &grid, &ray, &bin);
        CHECK(rc ==
              MLI_AXIS_ALIGNED_GRID_RAY_STARTS_OUTSIDE_GRID_BUT_INTERSECTS);
        CHECK(bin.x == 2);
        CHECK(bin.y == 1);
        CHECK(bin.z == 1);

        ray = mliRay_set(mli_Vec_init(-2, -2, -2), mli_Vec_init(-1, -1, -1));
        rc = mliAxisAlignedGrid_find_voxel_of_first_interaction(
                &grid, &ray, &bin);
        CHECK(rc == MLI_AXIS_ALIGNED_GRID_RAY_DOES_NOT_INTERSECT_GRID);

        ray = mliRay_set(mli_Vec_init(-2, -2, -2), mli_Vec_init(1, 1, 1));
        rc = mliAxisAlignedGrid_find_voxel_of_first_interaction(
                &grid, &ray, &bin);
        CHECK(rc ==
              MLI_AXIS_ALIGNED_GRID_RAY_STARTS_OUTSIDE_GRID_BUT_INTERSECTS);
        CHECK(bin.x == 0);
        CHECK(bin.y == 0);
        CHECK(bin.z == 0);

        ray = mliRay_set(mli_Vec_init(2, 2, 2), mli_Vec_init(-1, -1, -1));
        rc = mliAxisAlignedGrid_find_voxel_of_first_interaction(
                &grid, &ray, &bin);
        CHECK(rc ==
              MLI_AXIS_ALIGNED_GRID_RAY_STARTS_OUTSIDE_GRID_BUT_INTERSECTS);
        CHECK(bin.x == 2);
        CHECK(bin.y == 2);
        CHECK(bin.z == 2);

        ray = mliRay_set(mli_Vec_init(0, 2, 2), mli_Vec_init(0, -1, -1));
        rc = mliAxisAlignedGrid_find_voxel_of_first_interaction(
                &grid, &ray, &bin);
        CHECK(rc ==
              MLI_AXIS_ALIGNED_GRID_RAY_STARTS_OUTSIDE_GRID_BUT_INTERSECTS);
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
                mli_AABB_set(
                        mli_Vec_init(-2.5, -1.5, -1),
                        mli_Vec_init(2.5, 1.5, 1)),
                mliIdx3_set(5, 3, 2));
        CHECK_MARGIN(grid.bin_width.x, 1.0, 0.01);
        CHECK_MARGIN(grid.bin_width.y, 1.0, 0.01);
        CHECK_MARGIN(grid.bin_width.z, 1.0, 0.01);

        /* X-AXIS */
        ray = mliRay_set(mli_Vec_init(-3, 0.2, 0.1), mli_Vec_init(1, 0, 0));
        traversal = mliAxisAlignedGridTraversal_start(&grid, &ray);
        CHECK(traversal.valid);
        CHECK(traversal.voxel.x == 0);
        CHECK(traversal.voxel.y == 1);
        CHECK(traversal.voxel.z == 1);

        rc = mliAxisAlignedGridTraversal_next(&traversal);
        /*mliAxisAlignedGridTraversal_fprint(stderr, &traversal);*/
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
        ray = mliRay_set(mli_Vec_init(0.2, 8.3, 0.1), mli_Vec_init(0, -1, 0));
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
        ray = mliRay_set(
                mli_Vec_init(0.2, -0.1, -1223.0), mli_Vec_init(0, 0, 1));
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
        ray = mliRay_set(mli_Vec_init(0.0, 0.0, 0.5), mli_Vec_init(1, 0, 0));
        traversal = mliAxisAlignedGridTraversal_start(&grid, &ray);
        CHECK(traversal.valid);
        CHECK(traversal.voxel.x == 2);
        CHECK(traversal.voxel.y == 1);
        CHECK(traversal.voxel.z == 1);

        /*mliAxisAlignedGridTraversal_fprint(stderr, &traversal);*/
        rc = mliAxisAlignedGridTraversal_next(&traversal);
        /*mliAxisAlignedGridTraversal_fprint(stderr, &traversal);*/

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
        int not_too_often = 4;
        struct mliAxisAlignedGrid grid;
        struct mliAxisAlignedGridTraversal traversal;
        struct mliRay ray;

        grid = mliAxisAlignedGrid_set(
                mli_AABB_set(
                        mli_Vec_init(
                                -5119268.215672, -5119974.912158, -5000.000000),
                        mli_Vec_init(
                                5120731.784328, 5120025.087842, 5000.000000)),
                mliIdx3_set(1024, 1024, 1));

        /* okayish
        ray.support = mli_Vec_init(4.171968e+03, 4.857704e+03, 0.000000e+00);
        ray.direction = mli_Vec_init(-3.894984e-02,
        -6.049007e-01, 7.953478e-01);
        */
        /* oddish */
        ray.support = mli_Vec_init(8.808299e+04, 3.914948e+04, 0.000000e+00);
        ray.direction = mli_Vec_init(-1.900087e-02, 2.265741e-01, 9.738086e-01);

        traversal = mliAxisAlignedGridTraversal_start(&grid, &ray);
        mliAxisAlignedGridTraversal_fprint(stderr, &traversal);

        while (traversal.valid) {
                not_too_often -= 1;
                CHECK(not_too_often > 0);
                mliAxisAlignedGridTraversal_next(&traversal);
                mliAxisAlignedGridTraversal_fprint(stderr, &traversal);
        }
}

CASE("Elaborated example")
{
        int i, num_overlaps, num_rays, rc_aabb, rc_sphere;
        double overlaps_per_ray, minus_solution, plus_solution;
        struct mliAxisAlignedGrid grid;
        struct mliAxisAlignedGridTraversal traversal;
        struct mliRay ray;
        struct mli_Prng prng;
        struct mli_prng_UniformRange range;

        grid = mliAxisAlignedGrid_set(
                mli_AABB_set(mli_Vec_init(-3, -3, -3), mli_Vec_init(3, 3, 3)),
                mliIdx3_set(10, 10, 10));
        prng = mli_Prng_init_MT19937(0u);
        num_overlaps = 0;
        num_rays = 10000;
        range.start = -1.0;
        range.range = 2.0;

        for (i = 0; i < num_rays; i++) {
                struct mli_Vec ppp;
                struct mli_Vec ddd = mli_Vec_init(
                        mli_prng_draw_uniform(range, &prng),
                        mli_prng_draw_uniform(range, &prng),
                        mli_prng_draw_uniform(range, &prng));
                ddd = mli_Vec_normalized(ddd);
                ray = mliRay_set(
                        mli_Vec_multiply(ddd, 20), mli_Vec_multiply(ddd, -1.0));

                /*
                fprintf(stderr,
                        "i %d, s[%e, %e, %e] d[%e, %e, %e]\n",
                        i,
                        ray.support.x,
                        ray.support.y,
                        ray.support.z,
                        ray.direction.x,
                        ray.direction.y,
                        ray.direction.z
                );
                */

                ppp = mliRay_at(&ray, 20.0);

                CHECK_MARGIN(mli_Vec_norm(ppp), 0.0, 1e-3);

                rc_aabb = mliRay_has_overlap_aabb(ray, grid.bounds);
                rc_sphere = mliRay_sphere_intersection(
                        ray.support,
                        ray.direction,
                        3.0,
                        &minus_solution,
                        &plus_solution);

                CHECK(rc_sphere);

                /*fprintf(stderr, "para: %e, %d\n", para, rc_aabb);*/
                CHECK(rc_aabb);

                traversal = mliAxisAlignedGridTraversal_start(&grid, &ray);

                /*mliAxisAlignedGridTraversal_fprint(stderr, &traversal);*/

                CHECK(traversal.valid);
                num_overlaps += 1;

                while (traversal.valid) {
                        num_overlaps += 1;
                        mliAxisAlignedGridTraversal_next(&traversal);
                        /*mliAxisAlignedGridTraversal_fprint(stderr,
                         * &traversal);*/
                }
        }

        overlaps_per_ray = ((double)num_overlaps / (double)num_rays);
        CHECK(overlaps_per_ray >= 20);
        CHECK(overlaps_per_ray <= 23);
}
