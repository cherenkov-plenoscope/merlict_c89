/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mli_Quaternion, defaults")
{
        struct mli_Quaternion p = mli_Quaternion_set(1., 2., 3., 4.);
        CHECK(p.w = 1.);
        CHECK(p.x = 2.);
        CHECK(p.y = 3.);
        CHECK(p.z = 4.);
        CHECK_MARGIN(mli_Quaternion_norm(p), sqrt(1. + 4. + 9. + 16.), 1e-6);
}

CASE("rotation is pure i.e. norm is 1.")
{
        struct mli_Quaternion q = mli_Quaternion_set_rotaxis_and_angle(
                mli_Vec_set(1., 0., 0.), mli_math_deg2rad(23.));
        CHECK_MARGIN(mli_Quaternion_norm(q), 1., 1e-6);
}

CASE("unity quaternion and matrix")
{
        struct mli_Quaternion q_eye = mli_Quaternion_set_rotaxis_and_angle(
                mli_Vec_set(1., 0., 0.), mli_math_deg2rad(0.));
        struct mli_Mat r_eye = mli_Quaternion_to_matrix(q_eye);
        CHECK_MARGIN(r_eye.r00, 1., 1e-6);
        CHECK_MARGIN(r_eye.r01, 0., 1e-6);
        CHECK_MARGIN(r_eye.r02, 0., 1e-6);

        CHECK_MARGIN(r_eye.r10, 0., 1e-6);
        CHECK_MARGIN(r_eye.r11, 1., 1e-6);
        CHECK_MARGIN(r_eye.r12, 0., 1e-6);

        CHECK_MARGIN(r_eye.r20, 0., 1e-6);
        CHECK_MARGIN(r_eye.r21, 0., 1e-6);
        CHECK_MARGIN(r_eye.r22, 1., 1e-6);
}

CASE("rotation matrix z-axis +90deg")
{
        struct mli_Quaternion z90 = mli_Quaternion_set_rotaxis_and_angle(
                mli_Vec_set(0., 0., 1.), mli_math_deg2rad(90.));
        struct mli_Mat r_z90 = mli_Quaternion_to_matrix(z90);

        CHECK_MARGIN(r_z90.r00, 0., 1e-6);
        CHECK_MARGIN(r_z90.r01, -1., 1e-6);
        CHECK_MARGIN(r_z90.r02, 0., 1e-6);

        CHECK_MARGIN(r_z90.r10, 1., 1e-6);
        CHECK_MARGIN(r_z90.r11, 0., 1e-6);
        CHECK_MARGIN(r_z90.r12, 0., 1e-6);

        CHECK_MARGIN(r_z90.r20, 0., 1e-6);
        CHECK_MARGIN(r_z90.r21, 0., 1e-6);
        CHECK_MARGIN(r_z90.r22, 1., 1e-6);
}

CASE("sequence of rotations")
{
        /*
         *   __initial__
         *
         *              z
         *              |
         *              |
         *              |_____ y
         *             /
         *            /
         *           x
         *
         *   _axis x, +90deg__
         *
         *              y
         *              |
         *              |
         *      z ______|
         *             /
         *            /
         *           x
         *
         *   _axis z, +90deg
         *
         *              x
         *              |  y
         *              | /
         *      z ______|/
         *
         */
        struct mli_Quaternion x90 = mli_Quaternion_set_rotaxis_and_angle(
                mli_Vec_set(1., 0., 0.), mli_math_deg2rad(90.));
        struct mli_Quaternion z90 = mli_Quaternion_set_rotaxis_and_angle(
                mli_Vec_set(0., 0., 1.), mli_math_deg2rad(90.));
        struct mli_Quaternion z90_x90;
        struct mli_Mat r_z90_x90;
        CHECK_MARGIN(mli_Quaternion_norm(x90), 1., 1e-6);
        CHECK_MARGIN(mli_Quaternion_norm(z90), 1., 1e-6);
        z90_x90 = mli_Quaternion_product(x90, z90);
        CHECK_MARGIN(mli_Quaternion_norm(z90_x90), 1., 1e-6);
        r_z90_x90 = mli_Quaternion_to_matrix(z90_x90);

        CHECK_MARGIN(r_z90_x90.r00, 0., 1e-6);
        CHECK_MARGIN(r_z90_x90.r10, 0., 1e-6);
        CHECK_MARGIN(r_z90_x90.r20, 1., 1e-6);

        CHECK_MARGIN(r_z90_x90.r01, -1., 1e-6);
        CHECK_MARGIN(r_z90_x90.r11, 0., 1e-6);
        CHECK_MARGIN(r_z90_x90.r21, 0., 1e-6);

        CHECK_MARGIN(r_z90_x90.r02, 0., 1e-6);
        CHECK_MARGIN(r_z90_x90.r12, -1., 1e-6);
        CHECK_MARGIN(r_z90_x90.r22, 0., 1e-6);
}

CASE("Tait-Bryan-angles")
{
        struct mli_Quaternion q = mli_Quaternion_set_tait_bryan(0., 0., 0.);
        CHECK_MARGIN(mli_Quaternion_norm(q), 1., 1e-6);
}

CASE("Tait-Bryan-angles, 2")
{
        struct mli_Quaternion q =
                mli_Quaternion_set_tait_bryan(0., 0., mli_math_deg2rad(90));
        struct mli_Mat rot =
                mli_Mat_init_tait_bryan(0., 0., mli_math_deg2rad(90));
        struct mli_Mat rot_from_q;
        CHECK_MARGIN(mli_Quaternion_norm(q), 1., 1e-6);
        rot_from_q = mli_Quaternion_to_matrix(q);
        CHECK(mli_Mat_equal_margin(rot, rot_from_q, 1e-9));
}

CASE("multiple combinations")
{
        double rx, ry, rz;
        for (rx = -400; rx < 400; rx += 25) {
                for (ry = -400; ry < 400; ry += 25) {
                        for (rz = -400; rz < 400; rz += 25) {
                                double xrad = mli_math_deg2rad(rx);
                                double yrad = mli_math_deg2rad(ry);
                                double zrad = mli_math_deg2rad(rz);
                                struct mli_Quaternion q =
                                        mli_Quaternion_set_tait_bryan(
                                                xrad, yrad, zrad);
                                struct mli_Mat rot = mli_Mat_init_tait_bryan(
                                        xrad, yrad, zrad);
                                struct mli_Mat rot_from_q =
                                        mli_Quaternion_to_matrix(q);
                                CHECK(mli_Mat_equal_margin(
                                        rot, rot_from_q, 1e-9));
                        }
                }
        }
}
