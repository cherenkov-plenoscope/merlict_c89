/* Copyright 2019-2020 Sebastian Achim Mueller                                */

/* mliQuaternion */
{
    mliQuaternion p = mliQuaternion_set(1., 2., 3., 4.);
    CHECK(p.w = 1.);
    CHECK(p.x = 2.);
    CHECK(p.y = 3.);
    CHECK(p.z = 4.);
    CHECK_MARGIN(mliQuaternion_norm(p), sqrt(1.+4.+9.+16.), 1e-6);
}

/* mliQuaternion */
/* rotation is pure i.e. norm is 1. */
{
    mliQuaternion q = mliQuaternion_set_rotaxis_and_angle(
        mliVec_set(1., 0., 0.),
        mli_deg2rad(23.));
    CHECK_MARGIN(mliQuaternion_norm(q), 1., 1e-6);
}

/* mliQuaternion */
/* unity quaternion and matrix */
{
    mliQuaternion q_eye = mliQuaternion_set_rotaxis_and_angle(
        mliVec_set(1., 0., 0.),
        mli_deg2rad(0.));
    mliRotMat r_eye = mliQuaternion_to_matrix(q_eye);
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

/* mliQuaternion */
/* rotation matrix z-axis +90deg */
{
    mliQuaternion z90 = mliQuaternion_set_rotaxis_and_angle(
        mliVec_set(0., 0., 1.),
        mli_deg2rad(90.));
    mliRotMat r_z90 = mliQuaternion_to_matrix(z90);

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

/* mliQuaternion */
/* sequence of rotations */
{
    /*
    __initial__

                z
                |
                |
                |_____ y
               /
              /
             x

    __axis x, +90deg__

                y
                |
                |
        z ______|
               /
              /
             x

    __axis z, +90deg

                x
                |  y
                | /
        z ______|/

    */
    mliQuaternion x90 = mliQuaternion_set_rotaxis_and_angle(
        mliVec_set(1., 0., 0.),
        mli_deg2rad(90.));
    mliQuaternion z90 = mliQuaternion_set_rotaxis_and_angle(
        mliVec_set(0., 0., 1.),
        mli_deg2rad(90.));
    mliQuaternion z90_x90;
    mliRotMat r_z90_x90;
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

/* mliQuaternion */
/* Tait-Bryan-angles */
{
    mliQuaternion q = mliQuaternion_set_tait_bryan(0., 0.,0.);
    CHECK_MARGIN(mliQuaternion_norm(q), 1., 1e-6);
}

{
    mliQuaternion q = mliQuaternion_set_tait_bryan(0., 0., mli_deg2rad(90));
    mliRotMat rot = mliRotMat_init_tait_bryan(0., 0., mli_deg2rad(90));
    mliRotMat rot_from_q;
    CHECK_MARGIN(mliQuaternion_norm(q), 1., 1e-6);
    rot_from_q = mliQuaternion_to_matrix(q);
    CHECK(mliRotMat_equal_margin(rot, rot_from_q, 1e-9));
}

/* Tait-Bryan-angles */
/* multiple combinations */
{
    double rx, ry, rz;
    for (rx = -400; rx < 400; rx += 25) {
        for (ry = -400; ry < 400; ry += 25) {
            for (rz = -400; rz < 400; rz += 25) {
                double xrad = mli_deg2rad(rx);
                double yrad = mli_deg2rad(ry);
                double zrad = mli_deg2rad(rz);
                mliQuaternion q = mliQuaternion_set_tait_bryan(
                    xrad, yrad, zrad);
                mliRotMat rot = mliRotMat_init_tait_bryan(
                    xrad, yrad, zrad);
                mliRotMat rot_from_q = mliQuaternion_to_matrix(q);
                CHECK(mliRotMat_equal_margin(rot, rot_from_q, 1e-9));
            }
        }
    }
}
