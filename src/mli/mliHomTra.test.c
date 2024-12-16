/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("sequence, unity and unity shall be unity")
{
        struct mliHomTraComp a;
        struct mliHomTraComp b;
        struct mliHomTraComp a_b;

        a.translation = mli_Vec_init(0., 0., 0.);
        a.rotation = mliQuaternion_set_tait_bryan(0., 0., 0.);

        b.translation = mli_Vec_init(0., 0., 0.);
        b.rotation = mliQuaternion_set_tait_bryan(0., 0., 0.);

        a_b = mliHomTraComp_sequence(a, b);
        CHECK_MARGIN(a_b.translation.x, 0., 1e-9);
        CHECK_MARGIN(a_b.translation.y, 0., 1e-9);
        CHECK_MARGIN(a_b.translation.z, 0., 1e-9);

        CHECK_MARGIN(a_b.rotation.w, 1., 1e-9);
}

CASE("sequence, one translation, one rotation")
{
        struct mliHomTraComp a; /* only translation */
        struct mliHomTraComp b; /* only rotation */
        struct mliHomTraComp a_b;
        struct mliHomTra a_b_;

        a.translation = mli_Vec_init(0., 0., 1.);
        a.rotation = mliQuaternion_set_tait_bryan(0., 0., 0.);

        b.translation = mli_Vec_init(0., 0., 0.);
        b.rotation =
                mliQuaternion_set_tait_bryan(0., 0., mli_math_deg2rad(90.));

        a_b = mliHomTraComp_sequence(a, b);
        a_b_ = mliHomTra_from_compact(a_b);

        CHECK_MARGIN(a_b_.translation.x, 0., 1e-9);
        CHECK_MARGIN(a_b_.translation.y, 0., 1e-9);
        CHECK_MARGIN(a_b_.translation.z, 1., 1e-9);

        CHECK_MARGIN(a_b_.rotation.r00, 0., 1e-9);
        CHECK_MARGIN(a_b_.rotation.r10, -1., 1e-9);
        CHECK_MARGIN(a_b_.rotation.r20, 0., 1e-9);

        CHECK_MARGIN(a_b_.rotation.r01, 1., 1e-9);
        CHECK_MARGIN(a_b_.rotation.r11, 0., 1e-9);
        CHECK_MARGIN(a_b_.rotation.r21, 0., 1e-9);

        CHECK_MARGIN(a_b_.rotation.r02, 0., 1e-9);
        CHECK_MARGIN(a_b_.rotation.r12, 0., 1e-9);
        CHECK_MARGIN(a_b_.rotation.r22, 1., 1e-9);
}

CASE("sequence, cancelation")
{
        struct mliHomTraComp a;
        struct mliHomTraComp a_inverse; /* inverse ov a */
        struct mliHomTraComp a_a_inverse;
        struct mliHomTra a_a_inverse_;

        a.translation = mli_Vec_init(0., 0., 1.);
        a.rotation =
                mliQuaternion_set_tait_bryan(0., 0., -mli_math_deg2rad(90.));

        a_inverse.translation = mli_Vec_init(0., 0., -1.);
        a_inverse.rotation =
                mliQuaternion_set_tait_bryan(0., 0., mli_math_deg2rad(90.));

        a_a_inverse = mliHomTraComp_sequence(a, a_inverse);
        a_a_inverse_ = mliHomTra_from_compact(a_a_inverse);

        CHECK_MARGIN(a_a_inverse_.translation.x, 0., 1e-9);
        CHECK_MARGIN(a_a_inverse_.translation.y, 0., 1e-9);
        CHECK_MARGIN(a_a_inverse_.translation.z, 0., 1e-9);

        CHECK_MARGIN(a_a_inverse_.rotation.r00, 1., 1e-9);
        CHECK_MARGIN(a_a_inverse_.rotation.r10, 0., 1e-9);
        CHECK_MARGIN(a_a_inverse_.rotation.r20, 0., 1e-9);

        CHECK_MARGIN(a_a_inverse_.rotation.r01, 0., 1e-9);
        CHECK_MARGIN(a_a_inverse_.rotation.r11, 1., 1e-9);
        CHECK_MARGIN(a_a_inverse_.rotation.r21, 0., 1e-9);

        CHECK_MARGIN(a_a_inverse_.rotation.r02, 0., 1e-9);
        CHECK_MARGIN(a_a_inverse_.rotation.r12, 0., 1e-9);
        CHECK_MARGIN(a_a_inverse_.rotation.r22, 1., 1e-9);
}

CASE("only translate")
{
        struct mliHomTraComp t_AB;
        struct mliHomTra t_AB_;
        struct mli_Vec pA, pB, dA, dB;

        t_AB.translation = mli_Vec_init(0., 0., 1.);
        t_AB.rotation = mliQuaternion_set_tait_bryan(0., 0., 0.);
        t_AB_ = mliHomTra_from_compact(t_AB);

        pA = mli_Vec_init(0.0, 0.0, 1.0);
        pB = mliHomTra_pos(&t_AB_, pA);
        CHECK_MARGIN(pB.x, 0.0, 1e-9);
        CHECK_MARGIN(pB.y, 0.0, 1e-9);
        CHECK_MARGIN(pB.z, 2.0, 1e-9);

        dA = mli_Vec_init(1.0, 0.0, 0.0);
        dB = mliHomTra_dir(&t_AB_, dA);
        CHECK_MARGIN(dB.x, 1.0, 1e-9);
        CHECK_MARGIN(dB.y, 0.0, 1e-9);
        CHECK_MARGIN(dB.z, 0.0, 1e-9);
}

CASE("only rotation")
{
        struct mliHomTraComp t_AB;
        struct mliHomTra t_AB_;
        struct mli_Vec pA, pB, dA, dB;

        t_AB.translation = mli_Vec_init(0., 0., 0.);
        t_AB.rotation =
                mliQuaternion_set_tait_bryan(0., 0., mli_math_deg2rad(90.0));
        t_AB_ = mliHomTra_from_compact(t_AB);

        pA = mli_Vec_init(0.0, 0.0, 1.0);
        pB = mliHomTra_pos(&t_AB_, pA);
        CHECK_MARGIN(pB.x, 0.0, 1e-9);
        CHECK_MARGIN(pB.y, 0.0, 1e-9);
        CHECK_MARGIN(pB.z, 1.0, 1e-9);

        dA = mli_Vec_init(1.0, 0.0, 0.0);
        dB = mliHomTra_dir(&t_AB_, dA);
        CHECK_MARGIN(dB.x, 0.0, 1e-9);
        CHECK_MARGIN(dB.y, -1.0, 1e-9);
        CHECK_MARGIN(dB.z, 0.0, 1e-9);
}

CASE("complex sequence")
{
        uint64_t i;
        const uint64_t num_combinations = 100;
        struct mli_Prng prng = mli_Prng_init_MT19937(1337);

        struct mli_prng_UniformRange trans_range;
        struct mli_prng_UniformRange angle_range;
        trans_range.start = -10.0;
        trans_range.range = 20.0;
        angle_range.start = -MLI_MATH_PI;
        angle_range.range = 2.0 * MLI_MATH_PI;

        for (i = 0; i < num_combinations; i++) {
                struct mliHomTraComp _AB, _BC, _AC;
                struct mliHomTra AB, BC, AC;
                struct mli_Vec pos_B, pos_A, pos_C_via_B, pos_C_direct;

                pos_A = mli_Vec_init(
                        mli_prng_draw_uniform(trans_range, &prng),
                        mli_prng_draw_uniform(trans_range, &prng),
                        mli_prng_draw_uniform(trans_range, &prng));

                _AB.translation = mli_Vec_init(
                        mli_prng_draw_uniform(trans_range, &prng),
                        mli_prng_draw_uniform(trans_range, &prng),
                        mli_prng_draw_uniform(trans_range, &prng));
                _AB.rotation = mliQuaternion_set_tait_bryan(
                        mli_prng_draw_uniform(angle_range, &prng),
                        mli_prng_draw_uniform(angle_range, &prng),
                        mli_prng_draw_uniform(angle_range, &prng));
                AB = mliHomTra_from_compact(_AB);

                pos_B = mliHomTra_pos_inverse(&AB, pos_A);

                _BC.translation = mli_Vec_init(
                        mli_prng_draw_uniform(trans_range, &prng),
                        mli_prng_draw_uniform(trans_range, &prng),
                        mli_prng_draw_uniform(trans_range, &prng));
                _BC.rotation = mliQuaternion_set_tait_bryan(
                        mli_prng_draw_uniform(angle_range, &prng),
                        mli_prng_draw_uniform(angle_range, &prng),
                        mli_prng_draw_uniform(angle_range, &prng));
                BC = mliHomTra_from_compact(_BC);

                pos_C_via_B = mliHomTra_pos_inverse(&BC, pos_B);

                _AC = mliHomTraComp_sequence(_BC, _AB);
                AC = mliHomTra_from_compact(_AC);

                pos_C_direct = mliHomTra_pos_inverse(&AC, pos_A);
                CHECK_MARGIN(pos_C_direct.x, pos_C_via_B.x, 1e-9);
                CHECK_MARGIN(pos_C_direct.y, pos_C_via_B.y, 1e-9);
                CHECK_MARGIN(pos_C_direct.z, pos_C_via_B.z, 1e-9);
        }
}

CASE("mliMat_init_tait_bryan")
{
        struct mliMat rotation = mliMat_init_tait_bryan(0., 0., 0.);
        CHECK_MARGIN(rotation.r00, 1., 1e-9);
        CHECK_MARGIN(rotation.r01, 0., 1e-9);
        CHECK_MARGIN(rotation.r02, 0., 1e-9);

        CHECK_MARGIN(rotation.r10, 0., 1e-9);
        CHECK_MARGIN(rotation.r11, 1., 1e-9);
        CHECK_MARGIN(rotation.r12, 0., 1e-9);

        CHECK_MARGIN(rotation.r20, 0., 1e-9);
        CHECK_MARGIN(rotation.r21, 0., 1e-9);
        CHECK_MARGIN(rotation.r22, 1., 1e-9);

        rotation = mliMat_init_tait_bryan(0., 0., mli_math_deg2rad(90));
        CHECK_MARGIN(rotation.r00, 0., 1e-9);
        CHECK_MARGIN(rotation.r01, 1., 1e-9);
        CHECK_MARGIN(rotation.r02, 0., 1e-9);

        CHECK_MARGIN(rotation.r10, -1., 1e-9);
        CHECK_MARGIN(rotation.r11, 0., 1e-9);
        CHECK_MARGIN(rotation.r12, 0., 1e-9);

        CHECK_MARGIN(rotation.r20, 0., 1e-9);
        CHECK_MARGIN(rotation.r21, 0., 1e-9);
        CHECK_MARGIN(rotation.r22, 1., 1e-9);
        /*
         *   |y       rotation z 90deg   |x
         *   |           --->            |
         *   |                           |
         *   --------x            y-------
         */
}

CASE("mliMat_init_axis_angle")
{
        struct mliMat rotation;
        struct mli_Vec axis = {0., 0., 1.};
        rotation = mliMat_init_axis_angle(axis, 0.);
        CHECK_MARGIN(rotation.r00, 1., 1e-9);
        CHECK_MARGIN(rotation.r01, 0., 1e-9);
        CHECK_MARGIN(rotation.r02, 0., 1e-9);

        CHECK_MARGIN(rotation.r10, 0., 1e-9);
        CHECK_MARGIN(rotation.r11, 1., 1e-9);
        CHECK_MARGIN(rotation.r12, 0., 1e-9);

        CHECK_MARGIN(rotation.r20, 0., 1e-9);
        CHECK_MARGIN(rotation.r21, 0., 1e-9);
        CHECK_MARGIN(rotation.r22, 1., 1e-9);

        rotation = mliMat_init_axis_angle(axis, mli_math_deg2rad(90.));
        CHECK_MARGIN(rotation.r00, 0., 1e-9);
        CHECK_MARGIN(rotation.r01, 1., 1e-9);
        CHECK_MARGIN(rotation.r02, 0., 1e-9);

        CHECK_MARGIN(rotation.r10, -1., 1e-9);
        CHECK_MARGIN(rotation.r11, 0., 1e-9);
        CHECK_MARGIN(rotation.r12, 0., 1e-9);

        CHECK_MARGIN(rotation.r20, 0., 1e-9);
        CHECK_MARGIN(rotation.r21, 0., 1e-9);
        CHECK_MARGIN(rotation.r22, 1., 1e-9);
}

CASE("mliMat_init_tait_bryan, 45deg")
{
        struct mliMat rotation;
        struct mli_Vec a = {0., 0., 1.};
        struct mli_Vec a_rot;
        rotation = mliMat_init_tait_bryan(0., 0., mli_math_deg2rad(45.));
        a_rot = mli_transform_orientation(&rotation, a);
        CHECK_MARGIN(a_rot.x, 0., 1e-9);
        CHECK_MARGIN(a_rot.y, 0., 1e-9);
        CHECK_MARGIN(a_rot.z, 1., 1e-9);
}

CASE("mli_transform_orientation_inverse")
{
        struct mliMat rotation;
        struct mli_Vec x = {1., 0., 0.};
        struct mli_Vec x_rot;
        rotation = mliMat_init_tait_bryan(0., 0., mli_math_deg2rad(45.));
        x_rot = mli_transform_orientation(&rotation, x);
        CHECK_MARGIN(x_rot.x, 1. / sqrt(2.), 1e-6);
        CHECK_MARGIN(x_rot.y, -1. / sqrt(2.), 1e-6);
        CHECK_MARGIN(x_rot.z, 0., 1e-9);

        x_rot = mli_transform_orientation_inverse(&rotation, x);
        CHECK_MARGIN(x_rot.x, 1. / sqrt(2.), 1e-6);
        CHECK_MARGIN(x_rot.y, 1. / sqrt(2.), 1e-6);
        CHECK_MARGIN(x_rot.z, 0., 1e-9);
}

CASE("only translation, mli_transform_position_inverse")
{
        struct mliMat rotation;
        struct mli_Vec translation = {0., 0., 3.};
        struct mli_Vec x = {1., 0., 0.};
        struct mli_Vec x_t;
        rotation = mliMat_init_tait_bryan(0., 0., 0.);
        x_t = mli_transform_position(&rotation, translation, x);
        CHECK_MARGIN(x_t.x, 1., 1e-6);
        CHECK_MARGIN(x_t.y, 0., 1e-6);
        CHECK_MARGIN(x_t.z, 3., 1e-9);

        x_t = mli_transform_position_inverse(&rotation, translation, x);
        CHECK_MARGIN(x_t.x, 1., 1e-6);
        CHECK_MARGIN(x_t.y, 0., 1e-6);
        CHECK_MARGIN(x_t.z, -3., 1e-9);
}

CASE("trans_pos_forth_and_back_only_translation_component_set")
{
        struct mliHomTraComp homtra;
        struct mliHomTra homtra_;
        struct mli_Vec ux_original, ux_forth, ux_back;

        homtra.translation = mli_Vec_init(0.0, 0.0, 133.7);
        homtra.rotation = mliQuaternion_set_tait_bryan(0.0, 0.0, 0.0);
        homtra_ = mliHomTra_from_compact(homtra);

        ux_original = mli_Vec_init(1.0, 0.0, 0.0);
        ux_forth = mliHomTra_pos(&homtra_, ux_original);
        CHECK(1.0 == ux_forth.x);
        CHECK(0.0 == ux_forth.y);
        CHECK(133.7 == ux_forth.z);
        ux_back = mliHomTra_pos_inverse(&homtra_, ux_forth);
        CHECK(ux_original.x == ux_back.x);
        CHECK(ux_original.y == ux_back.y);
        CHECK(ux_original.z == ux_back.z);
}

CASE("transform_position_forth_and_back_full_set")
{
        struct mliHomTraComp homtra;
        struct mliHomTra homtra_;
        struct mli_Vec ux_original, ux_forth, ux_back;

        homtra.translation = mli_Vec_init(0.0, 0.0, 133.7);
        homtra.rotation = mliQuaternion_set_tait_bryan(0.0, MLI_MATH_PI, 0.0);
        homtra_ = mliHomTra_from_compact(homtra);

        ux_original = mli_Vec_init(1.0, 0.0, 0.0);
        ux_forth = mliHomTra_pos(&homtra_, ux_original);
        CHECK_MARGIN(-1.0, ux_forth.x, 1e-12);
        CHECK_MARGIN(0.0, ux_forth.y, 1e-12);
        CHECK_MARGIN(133.7, ux_forth.z, 1e-12);
        ux_back = mliHomTra_pos_inverse(&homtra_, ux_forth);
        CHECK(ux_original.x == ux_back.x);
        CHECK(ux_original.y == ux_back.y);
        CHECK(ux_original.z == ux_back.z);
}

CASE("trans_orientation_forth_and_back_only_rot_component_set")
{
        struct mliHomTraComp homtra;
        struct mliHomTra homtra_;
        struct mli_Vec ux_original, ux_forth, ux_back;

        homtra.translation = mli_Vec_init(0.0, 0.0, 0.0);
        homtra.rotation = mliQuaternion_set_tait_bryan(0.0, MLI_MATH_PI, 0.0);
        homtra_ = mliHomTra_from_compact(homtra);

        ux_original = mli_Vec_init(1.0, 0.0, 0.0);
        ux_forth = mliHomTra_dir(&homtra_, ux_original);
        CHECK_MARGIN(-1.0, ux_forth.x, 1e-12);
        CHECK_MARGIN(0.0, ux_forth.y, 1e-12);
        CHECK_MARGIN(0.0, ux_forth.z, 1e-12);
        ux_back = mliHomTra_dir_inverse(&homtra_, ux_forth);
        CHECK(ux_original.x == ux_back.x);
        CHECK(ux_original.y == ux_back.y);
        CHECK(ux_original.z == ux_back.z);
}
