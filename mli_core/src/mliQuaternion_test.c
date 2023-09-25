/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "../../mli_testing/include/mli_testing.h"
#include "mliQuaternion.h"

CASE("mliQuaternion, defaults")
{
        struct mliQuaternion p = mliQuaternion_set(1., 2., 3., 4.);
        CHECK(p.w = 1.);
        CHECK(p.x = 2.);
        CHECK(p.y = 3.);
        CHECK(p.z = 4.);
        CHECK_MARGIN(mliQuaternion_norm(p), sqrt(1. + 4. + 9. + 16.), 1e-6);
}

CASE("rotation is pure i.e. norm is 1.")
{
        struct mliQuaternion q = mliQuaternion_set_rotaxis_and_angle(
                mliVec_init(1., 0., 0.), mli_deg2rad(23.));
        CHECK_MARGIN(mliQuaternion_norm(q), 1., 1e-6);
}

CASE("unity quaternion and matrix")
{
        struct mliQuaternion q_eye = mliQuaternion_set_rotaxis_and_angle(
                mliVec_init(1., 0., 0.), mli_deg2rad(0.));
        struct mliMat r_eye = mliQuaternion_to_matrix(q_eye);
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
        struct mliQuaternion z90 = mliQuaternion_set_rotaxis_and_angle(
                mliVec_init(0., 0., 1.), mli_deg2rad(90.));
        struct mliMat r_z90 = mliQuaternion_to_matrix(z90);

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
        struct mliQuaternion x90 = mliQuaternion_set_rotaxis_and_angle(
                mliVec_init(1., 0., 0.), mli_deg2rad(90.));
        struct mliQuaternion z90 = mliQuaternion_set_rotaxis_and_angle(
                mliVec_init(0., 0., 1.), mli_deg2rad(90.));
        struct mliQuaternion z90_x90;
        struct mliMat r_z90_x90;
        CHECK_MARGIN(mliQuaternion_norm(x90), 1., 1e-6);
        CHECK_MARGIN(mliQuaternion_norm(z90), 1., 1e-6);
        z90_x90 = mliQuaternion_product(x90, z90);
        CHECK_MARGIN(mliQuaternion_norm(z90_x90), 1., 1e-6);
        r_z90_x90 = mliQuaternion_to_matrix(z90_x90);

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
        struct mliQuaternion q = mliQuaternion_set_tait_bryan(0., 0., 0.);
        CHECK_MARGIN(mliQuaternion_norm(q), 1., 1e-6);
}

CASE("Tait-Bryan-angles, 2")
{
        struct mliQuaternion q =
                mliQuaternion_set_tait_bryan(0., 0., mli_deg2rad(90));
        struct mliMat rot = mliMat_init_tait_bryan(0., 0., mli_deg2rad(90));
        struct mliMat rot_from_q;
        CHECK_MARGIN(mliQuaternion_norm(q), 1., 1e-6);
        rot_from_q = mliQuaternion_to_matrix(q);
        CHECK(mliMat_equal_margin(rot, rot_from_q, 1e-9));
}

CASE("multiple combinations")
{
        double rx, ry, rz;
        for (rx = -400; rx < 400; rx += 25) {
                for (ry = -400; ry < 400; ry += 25) {
                        for (rz = -400; rz < 400; rz += 25) {
                                double xrad = mli_deg2rad(rx);
                                double yrad = mli_deg2rad(ry);
                                double zrad = mli_deg2rad(rz);
                                struct mliQuaternion q =
                                        mliQuaternion_set_tait_bryan(
                                                xrad, yrad, zrad);
                                struct mliMat rot = mliMat_init_tait_bryan(
                                        xrad, yrad, zrad);
                                struct mliMat rot_from_q =
                                        mliQuaternion_to_matrix(q);
                                CHECK(mliMat_equal_margin(
                                        rot, rot_from_q, 1e-9));
                        }
                }
        }
}
