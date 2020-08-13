/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("sequence, unity and unity shall be unity")
{
        struct mliHomTraComp a;
        struct mliHomTraComp b;
        struct mliHomTraComp a_b;

        a.translation = mliVec_set(0., 0., 0.);
        a.rot = mliQuaternion_set_tait_bryan(0., 0., 0.);

        b.translation = mliVec_set(0., 0., 0.);
        b.rot = mliQuaternion_set_tait_bryan(0., 0., 0.);

        a_b = mliHomTraComp_sequence(a, b);
        CHECK_MARGIN(a_b.translation.x, 0., 1e-9);
        CHECK_MARGIN(a_b.translation.y, 0., 1e-9);
        CHECK_MARGIN(a_b.translation.z, 0., 1e-9);

        CHECK_MARGIN(a_b.rot.w, 1., 1e-9);
}

CASE("sequence, one translation, one rotation")
{
        struct mliHomTraComp a; /* only translation */
        struct mliHomTraComp b; /* only rotation */
        struct mliHomTraComp a_b;
        struct mliHomTra a_b_;

        a.translation = mliVec_set(0., 0., 1.);
        a.rot = mliQuaternion_set_tait_bryan(0., 0., 0.);

        b.translation = mliVec_set(0., 0., 0.);
        b.rot = mliQuaternion_set_tait_bryan(0., 0., mli_deg2rad(90.));

        a_b = mliHomTraComp_sequence(a, b);
        a_b_ = mliHomTra_from_compact(a_b);
        /* mliHomTra_print(a_b_); */
        CHECK_MARGIN(a_b_.translation.x, 0., 1e-9);
        CHECK_MARGIN(a_b_.translation.y, 0., 1e-9);
        CHECK_MARGIN(a_b_.translation.z, 1., 1e-9);

        CHECK_MARGIN(a_b_.rot.r00, 0., 1e-9);
        CHECK_MARGIN(a_b_.rot.r10, -1., 1e-9);
        CHECK_MARGIN(a_b_.rot.r20, 0., 1e-9);

        CHECK_MARGIN(a_b_.rot.r01, 1., 1e-9);
        CHECK_MARGIN(a_b_.rot.r11, 0., 1e-9);
        CHECK_MARGIN(a_b_.rot.r21, 0., 1e-9);

        CHECK_MARGIN(a_b_.rot.r02, 0., 1e-9);
        CHECK_MARGIN(a_b_.rot.r12, 0., 1e-9);
        CHECK_MARGIN(a_b_.rot.r22, 1., 1e-9);
}

CASE("sequence, cancelation")
{
        struct mliHomTraComp a;
        struct mliHomTraComp a_inverse; /* inverse ov a */
        struct mliHomTraComp a_a_inverse;
        struct mliHomTra a_a_inverse_;

        a.translation = mliVec_set(0., 0., 1.);
        a.rot = mliQuaternion_set_tait_bryan(0., 0., -mli_deg2rad(90.));

        a_inverse.translation = mliVec_set(0., 0., -1.);
        a_inverse.rot = mliQuaternion_set_tait_bryan(0., 0., mli_deg2rad(90.));

        a_a_inverse = mliHomTraComp_sequence(a, a_inverse);
        a_a_inverse_ = mliHomTra_from_compact(a_a_inverse);
        /* mliHomTra_print(a_a_inverse_); */
        CHECK_MARGIN(a_a_inverse_.translation.x, 0., 1e-9);
        CHECK_MARGIN(a_a_inverse_.translation.y, 0., 1e-9);
        CHECK_MARGIN(a_a_inverse_.translation.z, 0., 1e-9);

        CHECK_MARGIN(a_a_inverse_.rot.r00, 1., 1e-9);
        CHECK_MARGIN(a_a_inverse_.rot.r10, 0., 1e-9);
        CHECK_MARGIN(a_a_inverse_.rot.r20, 0., 1e-9);

        CHECK_MARGIN(a_a_inverse_.rot.r01, 0., 1e-9);
        CHECK_MARGIN(a_a_inverse_.rot.r11, 1., 1e-9);
        CHECK_MARGIN(a_a_inverse_.rot.r21, 0., 1e-9);

        CHECK_MARGIN(a_a_inverse_.rot.r02, 0., 1e-9);
        CHECK_MARGIN(a_a_inverse_.rot.r12, 0., 1e-9);
        CHECK_MARGIN(a_a_inverse_.rot.r22, 1., 1e-9);
}

CASE("simple sequence")
{
        struct mliHomTraComp a;
        struct mliHomTraComp b;
        struct mliHomTraComp a_b;
        struct mliHomTra a_b_;

        a.translation = mliVec_set(0., 0., 1.);
        a.rot = mliQuaternion_set_tait_bryan(0., 0., mli_deg2rad(90.));

        b.translation = mliVec_set(1., 0., 0.);
        b.rot = mliQuaternion_set_tait_bryan(mli_deg2rad(90.), 0., 0.);

        a_b = mliHomTraComp_sequence(a, b);
        a_b_ = mliHomTra_from_compact(a_b);
        /* mliHomTra_print(a_b_); */
        CHECK_MARGIN(a_b_.translation.x, 1., 1e-9);
        CHECK_MARGIN(a_b_.translation.y, 0., 1e-9);
        CHECK_MARGIN(a_b_.translation.z, 1., 1e-9);

        CHECK_MARGIN(a_b_.rot.r00, 0., 1e-9);
        CHECK_MARGIN(a_b_.rot.r10, -1., 1e-9);
        CHECK_MARGIN(a_b_.rot.r20, 0., 1e-9);

        CHECK_MARGIN(a_b_.rot.r01, 0., 1e-9);
        CHECK_MARGIN(a_b_.rot.r11, 0., 1e-9);
        CHECK_MARGIN(a_b_.rot.r21, -1., 1e-9);

        CHECK_MARGIN(a_b_.rot.r02, 1., 1e-9);
        CHECK_MARGIN(a_b_.rot.r12, 0., 1e-9);
        CHECK_MARGIN(a_b_.rot.r22, 0., 1e-9);
}

CASE("mliRotMat_init_tait_bryan")
{
        struct mliRotMat rot = mliRotMat_init_tait_bryan(0., 0., 0.);
        CHECK_MARGIN(rot.r00, 1., 1e-9);
        CHECK_MARGIN(rot.r01, 0., 1e-9);
        CHECK_MARGIN(rot.r02, 0., 1e-9);

        CHECK_MARGIN(rot.r10, 0., 1e-9);
        CHECK_MARGIN(rot.r11, 1., 1e-9);
        CHECK_MARGIN(rot.r12, 0., 1e-9);

        CHECK_MARGIN(rot.r20, 0., 1e-9);
        CHECK_MARGIN(rot.r21, 0., 1e-9);
        CHECK_MARGIN(rot.r22, 1., 1e-9);

        rot = mliRotMat_init_tait_bryan(0., 0., mli_deg2rad(90));
        CHECK_MARGIN(rot.r00, 0., 1e-9);
        CHECK_MARGIN(rot.r01, 1., 1e-9);
        CHECK_MARGIN(rot.r02, 0., 1e-9);

        CHECK_MARGIN(rot.r10, -1., 1e-9);
        CHECK_MARGIN(rot.r11, 0., 1e-9);
        CHECK_MARGIN(rot.r12, 0., 1e-9);

        CHECK_MARGIN(rot.r20, 0., 1e-9);
        CHECK_MARGIN(rot.r21, 0., 1e-9);
        CHECK_MARGIN(rot.r22, 1., 1e-9);
        /*
         *   |y       rot z 90deg        |x
         *   |           --->            |
         *   |                           |
         *   --------x            y-------
         */
}

CASE("mliRotMat_init_axis")
{
        struct mliRotMat rot;
        struct mliVec axis = {0., 0., 1.};
        rot = mliRotMat_init_axis(axis, 0.);
        CHECK_MARGIN(rot.r00, 1., 1e-9);
        CHECK_MARGIN(rot.r01, 0., 1e-9);
        CHECK_MARGIN(rot.r02, 0., 1e-9);

        CHECK_MARGIN(rot.r10, 0., 1e-9);
        CHECK_MARGIN(rot.r11, 1., 1e-9);
        CHECK_MARGIN(rot.r12, 0., 1e-9);

        CHECK_MARGIN(rot.r20, 0., 1e-9);
        CHECK_MARGIN(rot.r21, 0., 1e-9);
        CHECK_MARGIN(rot.r22, 1., 1e-9);

        rot = mliRotMat_init_axis(axis, mli_deg2rad(90.));
        CHECK_MARGIN(rot.r00, 0., 1e-9);
        CHECK_MARGIN(rot.r01, 1., 1e-9);
        CHECK_MARGIN(rot.r02, 0., 1e-9);

        CHECK_MARGIN(rot.r10, -1., 1e-9);
        CHECK_MARGIN(rot.r11, 0., 1e-9);
        CHECK_MARGIN(rot.r12, 0., 1e-9);

        CHECK_MARGIN(rot.r20, 0., 1e-9);
        CHECK_MARGIN(rot.r21, 0., 1e-9);
        CHECK_MARGIN(rot.r22, 1., 1e-9);
}

CASE("mliRotMat_init_tait_bryan, 45deg")
{
        struct mliRotMat rot;
        struct mliVec a = {0., 0., 1.};
        struct mliVec a_rot;
        rot = mliRotMat_init_tait_bryan(0., 0., mli_deg2rad(45.));
        a_rot = mli_transform_orientation(&rot, a);
        CHECK_MARGIN(a_rot.x, 0., 1e-9);
        CHECK_MARGIN(a_rot.y, 0., 1e-9);
        CHECK_MARGIN(a_rot.z, 1., 1e-9);
}

CASE("mli_transform_orientation_inverse")
{
        struct mliRotMat rot;
        struct mliVec x = {1., 0., 0.};
        struct mliVec x_rot;
        rot = mliRotMat_init_tait_bryan(0., 0., mli_deg2rad(45.));
        x_rot = mli_transform_orientation(&rot, x);
        CHECK_MARGIN(x_rot.x, 1. / sqrt(2.), 1e-6);
        CHECK_MARGIN(x_rot.y, -1. / sqrt(2.), 1e-6);
        CHECK_MARGIN(x_rot.z, 0., 1e-9);

        x_rot = mli_transform_orientation_inverse(&rot, x);
        CHECK_MARGIN(x_rot.x, 1. / sqrt(2.), 1e-6);
        CHECK_MARGIN(x_rot.y, 1. / sqrt(2.), 1e-6);
        CHECK_MARGIN(x_rot.z, 0., 1e-9);
}

CASE("only translation, mli_transform_position_inverse")
{
        struct mliRotMat rot;
        struct mliVec translation = {0., 0., 3.};
        struct mliVec x = {1., 0., 0.};
        struct mliVec x_t;
        rot = mliRotMat_init_tait_bryan(0., 0., 0.);
        x_t = mli_transform_position(&rot, translation, x);
        CHECK_MARGIN(x_t.x, 1., 1e-6);
        CHECK_MARGIN(x_t.y, 0., 1e-6);
        CHECK_MARGIN(x_t.z, 3., 1e-9);

        x_t = mli_transform_position_inverse(&rot, translation, x);
        CHECK_MARGIN(x_t.x, 1., 1e-6);
        CHECK_MARGIN(x_t.y, 0., 1e-6);
        CHECK_MARGIN(x_t.z, -3., 1e-9);
}
