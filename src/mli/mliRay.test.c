/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("ray and orientated bounding box")
{
        struct mliAABB aabb;
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

        double near, far;
        aabb.lower = mliVec_init(-1., -2, -3);
        aabb.upper = mliVec_init(1., -1, 0);

        /* ray starts below the box */
        mliRay_aabb_intersections(
                mliRay_set(mliVec_init(0., -1.5, -4.), mliVec_init(0., 0., 1.)),
                aabb,
                &near,
                &far);
        CHECK(mliRay_aabb_intersections_is_valid_given_near_and_far(near, far));
        CHECK_MARGIN(near, 1., 1e-6);
        CHECK_MARGIN(far, 4., 1e-6);

        /* ray starts above the box */
        mliRay_aabb_intersections(
                mliRay_set(mliVec_init(0., -1.5, +4.), mliVec_init(0., 0., 1.)),
                aabb,
                &near,
                &far);
        CHECK(!mliRay_aabb_intersections_is_valid_given_near_and_far(
                near, far));

        /* ray starts inside the box */
        mliRay_aabb_intersections(
                mliRay_set(
                        mliVec_init(0., -1.5, -1.5), mliVec_init(0., 0., 1.)),
                aabb,
                &near,
                &far);
        CHECK(mliRay_aabb_intersections_is_valid_given_near_and_far(near, far));
        CHECK_MARGIN(near, -1.5, 1e-6);
        CHECK_MARGIN(far, 1.5, 1e-6);

        /* ray starts beside the box */
        mliRay_aabb_intersections(
                mliRay_set(mliVec_init(10, 10, -5), mliVec_init(0., 0., 1.)),
                aabb,
                &near,
                &far);
        CHECK(!mliRay_aabb_intersections_is_valid_given_near_and_far(
                near, far));
}

CASE("ray inside aabb")
{
        struct mliAABB aabb;
        struct mliVec direction;
        double near, far;
        int i;
        aabb.lower = mliVec_init(-1, -1, -1);
        aabb.upper = mliVec_init(1, 1, 1);

        /* ray starts inside the box */
        for (i = 0; i < 6; i++) {
                switch (i) {
                case 0:
                        direction = mliVec_init(-1, 0, 0);
                        break;
                case 1:
                        direction = mliVec_init(1, 0, 0);
                        break;
                case 2:
                        direction = mliVec_init(0, -1, 0);
                        break;
                case 3:
                        direction = mliVec_init(0, 1, 0);
                        break;
                case 4:
                        direction = mliVec_init(0, 0, -1);
                        break;
                case 5:
                        direction = mliVec_init(0, 0, 1);
                        break;
                default:
                        CHECK(0);
                }
                mliRay_aabb_intersections(
                        mliRay_set(mliVec_init(0., 0., 0.), direction),
                        aabb,
                        &near,
                        &far);
                CHECK(mliRay_aabb_intersections_is_valid_given_near_and_far(
                        near, far));
                CHECK_MARGIN(near, -1., 1e-6);
                CHECK_MARGIN(far, 1., 1e-6);
        }
}

CASE("ray aabb actual test cases")
{
        struct mliAABB aabb;
        /*struct mliVec aabbextent;*/
        struct mliRay ray;
        /*struct mliVec vn, vf;*/
        double near, far;
        aabb.lower = mliVec_init(-5124735.0, -5120543.0, -5000.0);
        aabb.upper = mliVec_init(5115265.0, 5119457.0, 5000.0);
        /*aabbextent = mliVec_substract(aabb.upper, aabb.lower);*/
        ray = mliRay_set(
                mliVec_init(-15220843.0, -16454620.0, 0.0),
                mliVec_init(0.678895, 0.734165, 0.010214));

        mliRay_aabb_intersections(ray, aabb, &near, &far);
        CHECK(!mliRay_aabb_intersections_is_valid_given_near_and_far(
                near, far));
        /*
        vn = mliRay_at(&ray, near);
        vf = mliRay_at(&ray, far);
        fprintf(stderr, "n:%e, f:%e\n", near, far);
        fprintf(stderr, "vn:(%f, %f, %f), vf:(%f, %f, %f)\n",
                vn.x/aabbextent.x, vn.y/aabbextent.y, vn.z/aabbextent.z,
                vf.x/aabbextent.x, vf.y/aabbextent.y, vf.z/aabbextent.z);
        */
}

CASE("mliHomTraComp, transform direction")
{
        struct mliHomTraComp Tcomp;
        struct mliHomTra T;
        struct mliVec v1, v2, v3;
        Tcomp.translation = mliVec_init(1., 0., 0.);
        Tcomp.rotation = mliQuaternion_set_rotaxis_and_angle(
                mliVec_init(0., 0., 1.), mtl_math_deg2rad(90));
        T = mliHomTra_from_compact(Tcomp);
        v1 = mliVec_init(1., 0., 0.);
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
                mliRay_set(mliVec_init(0., 0., 1.), mliVec_init(0., 0., 1.));
        struct mliRay ray2;
        struct mliHomTraComp Tcomp;
        struct mliHomTra T;
        Tcomp.translation = mliVec_init(0., 0., 0.);
        Tcomp.rotation = mliQuaternion_set_tait_bryan(0., 0., 0.);
        T = mliHomTra_from_compact(Tcomp);
        ray2 = mliHomTra_ray(&T, ray);
        CHECK(mliVec_equal_margin(ray2.support, ray.support, 1e-6));
        CHECK(mliVec_equal_margin(ray2.direction, ray.direction, 1e-6));
}

CASE("translation")
{
        struct mliRay ray =
                mliRay_set(mliVec_init(0., 0., 1.), mliVec_init(0., 0., 1.));
        struct mliRay ray2;
        struct mliHomTraComp Tcomp;
        struct mliHomTra T;
        Tcomp.translation = mliVec_init(1., 0., 0.);
        Tcomp.rotation = mliQuaternion_set_rotaxis_and_angle(
                mliVec_init(0., 0., 1.), 0.);
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
                mliRay_set(mliVec_init(0., 0., 1.), mliVec_init(1., 0., 0.));
        struct mliRay ray2;
        struct mliHomTraComp Tcomp;
        struct mliHomTra T;
        Tcomp.translation = mliVec_init(0., 0., 0.);
        Tcomp.rotation = mliQuaternion_set_rotaxis_and_angle(
                mliVec_init(0., 0., 1.), mtl_math_deg2rad(90));
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
                mliRay_set(mliVec_init(0., 0., 1.), mliVec_init(1., 0., 0.));
        struct mliRay ray2;
        struct mliRay ray3;
        struct mliHomTraComp Tcomp;
        struct mliHomTra T;
        Tcomp.translation = mliVec_init(0., 1., 0.);
        Tcomp.rotation = mliQuaternion_set_rotaxis_and_angle(
                mliVec_init(0., 0., 1.), mtl_math_deg2rad(90));
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
