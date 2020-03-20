/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("ray and orientated bounding box")
{
        struct mliOBB obb;
        /*
         *         (-1, -2,  0)                         (-1, -1,  0)
         *                     O----------------------O
         *                    /.                     /|
         *                   / .                    / |
         *                  /  .                   /  |
         *                 /   .                  /   |
         *   ( 1, -2,  0) O----------------------O ( 1, -1,  0)
         *                |    .                 |    |
         *      Z         |    .                 |    |
         *      | (-1, -2,|-3) O- - - - - - - - -| - -O (-1, -1, -3)
         *      |         |   .                  |   /
         *      |         |  .                   |  /
         *      /-----Y   | .                    | /
         *     /          |.                     |/
         *    X           O----------------------O
         *     ( 1, -2, -3)                       ( 1, -1, -3)
         */

        double ray_parameter;
        obb.lower = mliVec_set(-1., -2, -3);
        obb.upper = mliVec_set(1., -1, 0);

        /* ray starts below the box */
        CHECK(mliRay_has_overlap_obb(
                mliRay_set(mliVec_set(0., -1.5, -4.), mliVec_set(0., 0., 1.)),
                obb,
                &ray_parameter));
        CHECK_MARGIN(ray_parameter, 1., 1e-6);

        /* ray starts above the box */
        CHECK(!mliRay_has_overlap_obb(
                mliRay_set(mliVec_set(0., -1.5, +4.), mliVec_set(0., 0., 1.)),
                obb,
                &ray_parameter));

        /* ray starts inside the box */
        CHECK(mliRay_has_overlap_obb(
                mliRay_set(mliVec_set(0., -1.5, -1.5), mliVec_set(0., 0., 1.)),
                obb,
                &ray_parameter));
        CHECK_MARGIN(ray_parameter, -1.5, 1e-6);

        /* ray starts beside the box */
        CHECK(!mliRay_has_overlap_obb(
                mliRay_set(mliVec_set(10, 10, -5), mliVec_set(0., 0., 1.)),
                obb,
                &ray_parameter));
}

CASE("ray has causal intersection")
{
        double causal_solution;
        int rc;
        struct mliBoundSurfaceChecklist cl;

        /*
         *       _/_ps
         *      //  \
         *     |/ ms |
         *     /\___/
         *    /
         *   X support
         */
        cl.plus_solution = 2.0;
        cl.minus_solution = 1.0;
        cl.plus_is_inside = 1;
        cl.minus_is_inside = 1;
        rc = mli_outer_bound_surface_causal_intersection(cl, &causal_solution);
        CHECK(causal_solution == cl.minus_solution);
        CHECK(rc);

        /*
         *       _/_ps
         *      //  \
         *     |X support
         *  ms /\___/
         *    /
         *
         */
        cl.plus_solution = 1.0;
        cl.minus_solution = -1.0;
        cl.plus_is_inside = 1;
        cl.minus_is_inside = 1;
        rc = mli_outer_bound_surface_causal_intersection(cl, &causal_solution);
        CHECK(causal_solution == cl.plus_solution);
        CHECK(rc);

        /*
         *          /
         *         X support
         *       _/_ps
         *      //  \
         *     |/    |
         *  ms /\___/
         *    /
         *
         */
        cl.plus_solution = -1.0;
        cl.minus_solution = -2.0;
        cl.plus_is_inside = 1;
        cl.minus_is_inside = 1;
        rc = mli_outer_bound_surface_causal_intersection(cl, &causal_solution);
        CHECK(!rc);
}

CASE("mliSphericalCapHex, bounding radius")
{
        struct mliSphericalCapHex cap;
        double bounding_radius;
        cap.curvature_radius = 1.0;
        cap.inner_hex_radius = .1;
        bounding_radius = mliSphericalCapHex_bounding_radius(cap);
        CHECK(bounding_radius > cap.inner_hex_radius * MLI_2_OVER_SQRT3);
        CHECK(bounding_radius < 1.1 * cap.inner_hex_radius * MLI_2_OVER_SQRT3);
}

CASE("mliHomTraComp, transform direction")
{
        struct mliHomTraComp Tcomp;
        struct mliHomTra T;
        struct mliVec v1, v2, v3;
        Tcomp.trans = mliVec_set(1., 0., 0.);
        Tcomp.rot = mliQuaternion_set_rotaxis_and_angle(
                mliVec_set(0., 0., 1.), mli_deg2rad(90));
        T = mliHomTra_from_compact(Tcomp);
        v1 = mliVec_set(1., 0., 0.);
        v2 = mliHomTra_dir(&T, v1);
        CHECK_MARGIN(v2.x, 0., 1e-6);
        CHECK_MARGIN(v2.y, 1., 1e-6);
        CHECK_MARGIN(v2.z, 0., 1e-6);

        v3 = mliHomTra_dir_inverse(&T, v2);
        CHECK_MARGIN(v3.x, v1.x, 1e-6);
        CHECK_MARGIN(v3.y, v1.y, 1e-6);
        CHECK_MARGIN(v3.z, v1.z, 1e-6);
}

CASE("unity transformation must not change ray")
{
        struct mliRay ray =
                mliRay_set(mliVec_set(0., 0., 1.), mliVec_set(0., 0., 1.));
        struct mliRay ray2;
        struct mliHomTraComp Tcomp;
        struct mliHomTra T;
        Tcomp.trans = mliVec_set(0., 0., 0.);
        Tcomp.rot =
                mliQuaternion_set_rotaxis_and_angle(mliVec_set(0., 0., 0.), 0.);
        T = mliHomTra_from_compact(Tcomp);
        ray2 = mliHomTra_ray(&T, ray);
        CHECK(mliVec_equal_margin(ray2.support, ray.support, 1e-6));
        CHECK(mliVec_equal_margin(ray2.direction, ray.direction, 1e-6));
}

CASE("translation")
{
        struct mliRay ray =
                mliRay_set(mliVec_set(0., 0., 1.), mliVec_set(0., 0., 1.));
        struct mliRay ray2;
        struct mliHomTraComp Tcomp;
        struct mliHomTra T;
        Tcomp.trans = mliVec_set(1., 0., 0.);
        Tcomp.rot =
                mliQuaternion_set_rotaxis_and_angle(mliVec_set(0., 0., 1.), 0.);
        T = mliHomTra_from_compact(Tcomp);
        ray2 = mliHomTra_ray(&T, ray);
        CHECK(ray2.support.x == 1.);
        CHECK(ray2.support.y == 0.);
        CHECK(ray2.support.z == 1.);
        CHECK(mliVec_equal_margin(ray2.direction, ray.direction, 1e-6));
}

CASE("rotation")
{
        struct mliRay ray =
                mliRay_set(mliVec_set(0., 0., 1.), mliVec_set(1., 0., 0.));
        struct mliRay ray2;
        struct mliHomTraComp Tcomp;
        struct mliHomTra T;
        Tcomp.trans = mliVec_set(0., 0., 0.);
        Tcomp.rot = mliQuaternion_set_rotaxis_and_angle(
                mliVec_set(0., 0., 1.), mli_deg2rad(90));
        T = mliHomTra_from_compact(Tcomp);
        ray2 = mliHomTra_ray(&T, ray);
        CHECK(mliVec_equal_margin(ray2.support, ray.support, 1e-6));
        CHECK_MARGIN(ray2.direction.x, 0., 1e-6);
        CHECK_MARGIN(ray2.direction.y, 1., 1e-6);
        CHECK_MARGIN(ray2.direction.z, 0., 1e-6);
}

CASE("translation and rotation")
{
        struct mliRay ray =
                mliRay_set(mliVec_set(0., 0., 1.), mliVec_set(1., 0., 0.));
        struct mliRay ray2;
        struct mliRay ray3;
        struct mliHomTraComp Tcomp;
        struct mliHomTra T;
        Tcomp.trans = mliVec_set(0., 1., 0.);
        Tcomp.rot = mliQuaternion_set_rotaxis_and_angle(
                mliVec_set(0., 0., 1.), mli_deg2rad(90));
        T = mliHomTra_from_compact(Tcomp);
        ray2 = mliHomTra_ray(&T, ray);
        CHECK_MARGIN(ray2.support.x, 0., 1e-6);
        CHECK_MARGIN(ray2.support.y, 1., 1e-6);
        CHECK_MARGIN(ray2.support.z, 1., 1e-6);
        CHECK_MARGIN(ray2.direction.x, 0., 1e-6);
        CHECK_MARGIN(ray2.direction.y, 1., 1e-6);
        CHECK_MARGIN(ray2.direction.z, 0., 1e-6);

        /* and back again */
        ray3.support = mliHomTra_pos_inverse(&T, ray2.support);
        ray3.direction = mliHomTra_dir_inverse(&T, ray2.direction);
        CHECK_MARGIN(ray3.support.x, ray.support.x, 1e-6);
        CHECK_MARGIN(ray3.support.y, ray.support.y, 1e-6);
        CHECK_MARGIN(ray3.support.z, ray.support.z, 1e-6);
        CHECK_MARGIN(ray3.direction.x, ray.direction.x, 1e-6);
        CHECK_MARGIN(ray3.direction.y, ray.direction.y, 1e-6);
        CHECK_MARGIN(ray3.direction.z, ray.direction.z, 1e-6);
}

CASE("mli_xyplane_equation")
{
        double solution;
        CHECK(mli_xyplane_equation(
                mliRay_set(mliVec_set(0., 0., 1.), mliVec_set(0., 0., -1.)),
                &solution));
        CHECK_MARGIN(solution, 1., 1e-6);

        CHECK(mli_xyplane_equation(
                mliRay_set(mliVec_set(0., 0., 1.), mliVec_set(0., 1., -1.)),
                &solution));
        CHECK_MARGIN(solution, sqrt(2.), 1e-6);

        CHECK(!mli_xyplane_equation(
                mliRay_set(mliVec_set(0., 0., 1.), mliVec_set(0., 1., 0.)),
                &solution));
}
