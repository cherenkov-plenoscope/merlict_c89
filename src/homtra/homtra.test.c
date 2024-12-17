/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("sequence, unity and unity shall be unity")
{
        struct mli_HomTraComp a;
        struct mli_HomTraComp b;
        struct mli_HomTraComp a_b;

        a.translation = mli_Vec_init(0., 0., 0.);
        a.rotation = mli_Quaternion_set_tait_bryan(0., 0., 0.);

        b.translation = mli_Vec_init(0., 0., 0.);
        b.rotation = mli_Quaternion_set_tait_bryan(0., 0., 0.);

        a_b = mli_HomTraComp_sequence(a, b);
        CHECK_MARGIN(a_b.translation.x, 0., 1e-9);
        CHECK_MARGIN(a_b.translation.y, 0., 1e-9);
        CHECK_MARGIN(a_b.translation.z, 0., 1e-9);

        CHECK_MARGIN(a_b.rotation.w, 1., 1e-9);
}

CASE("sequence, one translation, one rotation")
{
        struct mli_HomTraComp a; /* only translation */
        struct mli_HomTraComp b; /* only rotation */
        struct mli_HomTraComp a_b;
        struct mli_HomTra a_b_;

        a.translation = mli_Vec_init(0., 0., 1.);
        a.rotation = mli_Quaternion_set_tait_bryan(0., 0., 0.);

        b.translation = mli_Vec_init(0., 0., 0.);
        b.rotation =
                mli_Quaternion_set_tait_bryan(0., 0., mli_math_deg2rad(90.));

        a_b = mli_HomTraComp_sequence(a, b);
        a_b_ = mli_HomTraComp_from_compact(a_b);

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
        struct mli_HomTraComp a;
        struct mli_HomTraComp a_inverse; /* inverse ov a */
        struct mli_HomTraComp a_a_inverse;
        struct mli_HomTra a_a_inverse_;

        a.translation = mli_Vec_init(0., 0., 1.);
        a.rotation =
                mli_Quaternion_set_tait_bryan(0., 0., -mli_math_deg2rad(90.));

        a_inverse.translation = mli_Vec_init(0., 0., -1.);
        a_inverse.rotation =
                mli_Quaternion_set_tait_bryan(0., 0., mli_math_deg2rad(90.));

        a_a_inverse = mli_HomTraComp_sequence(a, a_inverse);
        a_a_inverse_ = mli_HomTraComp_from_compact(a_a_inverse);

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
        struct mli_HomTraComp t_AB;
        struct mli_HomTra t_AB_;
        struct mli_Vec pA, pB, dA, dB;

        t_AB.translation = mli_Vec_init(0., 0., 1.);
        t_AB.rotation = mli_Quaternion_set_tait_bryan(0., 0., 0.);
        t_AB_ = mli_HomTraComp_from_compact(t_AB);

        pA = mli_Vec_init(0.0, 0.0, 1.0);
        pB = mli_HomTraComp_pos(&t_AB_, pA);
        CHECK_MARGIN(pB.x, 0.0, 1e-9);
        CHECK_MARGIN(pB.y, 0.0, 1e-9);
        CHECK_MARGIN(pB.z, 2.0, 1e-9);

        dA = mli_Vec_init(1.0, 0.0, 0.0);
        dB = mli_HomTraComp_dir(&t_AB_, dA);
        CHECK_MARGIN(dB.x, 1.0, 1e-9);
        CHECK_MARGIN(dB.y, 0.0, 1e-9);
        CHECK_MARGIN(dB.z, 0.0, 1e-9);
}

CASE("only rotation")
{
        struct mli_HomTraComp t_AB;
        struct mli_HomTra t_AB_;
        struct mli_Vec pA, pB, dA, dB;

        t_AB.translation = mli_Vec_init(0., 0., 0.);
        t_AB.rotation =
                mli_Quaternion_set_tait_bryan(0., 0., mli_math_deg2rad(90.0));
        t_AB_ = mli_HomTraComp_from_compact(t_AB);

        pA = mli_Vec_init(0.0, 0.0, 1.0);
        pB = mli_HomTraComp_pos(&t_AB_, pA);
        CHECK_MARGIN(pB.x, 0.0, 1e-9);
        CHECK_MARGIN(pB.y, 0.0, 1e-9);
        CHECK_MARGIN(pB.z, 1.0, 1e-9);

        dA = mli_Vec_init(1.0, 0.0, 0.0);
        dB = mli_HomTraComp_dir(&t_AB_, dA);
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
                struct mli_HomTraComp _AB, _BC, _AC;
                struct mli_HomTra AB, BC, AC;
                struct mli_Vec pos_B, pos_A, pos_C_via_B, pos_C_direct;

                pos_A = mli_Vec_init(
                        mli_prng_draw_uniform(trans_range, &prng),
                        mli_prng_draw_uniform(trans_range, &prng),
                        mli_prng_draw_uniform(trans_range, &prng));

                _AB.translation = mli_Vec_init(
                        mli_prng_draw_uniform(trans_range, &prng),
                        mli_prng_draw_uniform(trans_range, &prng),
                        mli_prng_draw_uniform(trans_range, &prng));
                _AB.rotation = mli_Quaternion_set_tait_bryan(
                        mli_prng_draw_uniform(angle_range, &prng),
                        mli_prng_draw_uniform(angle_range, &prng),
                        mli_prng_draw_uniform(angle_range, &prng));
                AB = mli_HomTraComp_from_compact(_AB);

                pos_B = mli_HomTraComp_pos_inverse(&AB, pos_A);

                _BC.translation = mli_Vec_init(
                        mli_prng_draw_uniform(trans_range, &prng),
                        mli_prng_draw_uniform(trans_range, &prng),
                        mli_prng_draw_uniform(trans_range, &prng));
                _BC.rotation = mli_Quaternion_set_tait_bryan(
                        mli_prng_draw_uniform(angle_range, &prng),
                        mli_prng_draw_uniform(angle_range, &prng),
                        mli_prng_draw_uniform(angle_range, &prng));
                BC = mli_HomTraComp_from_compact(_BC);

                pos_C_via_B = mli_HomTraComp_pos_inverse(&BC, pos_B);

                _AC = mli_HomTraComp_sequence(_BC, _AB);
                AC = mli_HomTraComp_from_compact(_AC);

                pos_C_direct = mli_HomTraComp_pos_inverse(&AC, pos_A);
                CHECK_MARGIN(pos_C_direct.x, pos_C_via_B.x, 1e-9);
                CHECK_MARGIN(pos_C_direct.y, pos_C_via_B.y, 1e-9);
                CHECK_MARGIN(pos_C_direct.z, pos_C_via_B.z, 1e-9);
        }
}

CASE("mli_Mat_init_tait_bryan")
{
        struct mli_Mat rotation = mli_Mat_init_tait_bryan(0., 0., 0.);
        CHECK_MARGIN(rotation.r00, 1., 1e-9);
        CHECK_MARGIN(rotation.r01, 0., 1e-9);
        CHECK_MARGIN(rotation.r02, 0., 1e-9);

        CHECK_MARGIN(rotation.r10, 0., 1e-9);
        CHECK_MARGIN(rotation.r11, 1., 1e-9);
        CHECK_MARGIN(rotation.r12, 0., 1e-9);

        CHECK_MARGIN(rotation.r20, 0., 1e-9);
        CHECK_MARGIN(rotation.r21, 0., 1e-9);
        CHECK_MARGIN(rotation.r22, 1., 1e-9);

        rotation = mli_Mat_init_tait_bryan(0., 0., mli_math_deg2rad(90));
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

CASE("mli_Mat_init_axis_angle")
{
        struct mli_Mat rotation;
        struct mli_Vec axis = {0., 0., 1.};
        rotation = mli_Mat_init_axis_angle(axis, 0.);
        CHECK_MARGIN(rotation.r00, 1., 1e-9);
        CHECK_MARGIN(rotation.r01, 0., 1e-9);
        CHECK_MARGIN(rotation.r02, 0., 1e-9);

        CHECK_MARGIN(rotation.r10, 0., 1e-9);
        CHECK_MARGIN(rotation.r11, 1., 1e-9);
        CHECK_MARGIN(rotation.r12, 0., 1e-9);

        CHECK_MARGIN(rotation.r20, 0., 1e-9);
        CHECK_MARGIN(rotation.r21, 0., 1e-9);
        CHECK_MARGIN(rotation.r22, 1., 1e-9);

        rotation = mli_Mat_init_axis_angle(axis, mli_math_deg2rad(90.));
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

CASE("mli_Mat_init_tait_bryan, 45deg")
{
        struct mli_Mat rotation;
        struct mli_Vec a = {0., 0., 1.};
        struct mli_Vec a_rot;
        rotation = mli_Mat_init_tait_bryan(0., 0., mli_math_deg2rad(45.));
        a_rot = mli_transform_orientation(&rotation, a);
        CHECK_MARGIN(a_rot.x, 0., 1e-9);
        CHECK_MARGIN(a_rot.y, 0., 1e-9);
        CHECK_MARGIN(a_rot.z, 1., 1e-9);
}

CASE("mli_transform_orientation_inverse")
{
        struct mli_Mat rotation;
        struct mli_Vec x = {1., 0., 0.};
        struct mli_Vec x_rot;
        rotation = mli_Mat_init_tait_bryan(0., 0., mli_math_deg2rad(45.));
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
        struct mli_Mat rotation;
        struct mli_Vec translation = {0., 0., 3.};
        struct mli_Vec x = {1., 0., 0.};
        struct mli_Vec x_t;
        rotation = mli_Mat_init_tait_bryan(0., 0., 0.);
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
        struct mli_HomTraComp homtra;
        struct mli_HomTra homtra_;
        struct mli_Vec ux_original, ux_forth, ux_back;

        homtra.translation = mli_Vec_init(0.0, 0.0, 133.7);
        homtra.rotation = mli_Quaternion_set_tait_bryan(0.0, 0.0, 0.0);
        homtra_ = mli_HomTraComp_from_compact(homtra);

        ux_original = mli_Vec_init(1.0, 0.0, 0.0);
        ux_forth = mli_HomTraComp_pos(&homtra_, ux_original);
        CHECK(1.0 == ux_forth.x);
        CHECK(0.0 == ux_forth.y);
        CHECK(133.7 == ux_forth.z);
        ux_back = mli_HomTraComp_pos_inverse(&homtra_, ux_forth);
        CHECK(ux_original.x == ux_back.x);
        CHECK(ux_original.y == ux_back.y);
        CHECK(ux_original.z == ux_back.z);
}

CASE("transform_position_forth_and_back_full_set")
{
        struct mli_HomTraComp homtra;
        struct mli_HomTra homtra_;
        struct mli_Vec ux_original, ux_forth, ux_back;

        homtra.translation = mli_Vec_init(0.0, 0.0, 133.7);
        homtra.rotation = mli_Quaternion_set_tait_bryan(0.0, MLI_MATH_PI, 0.0);
        homtra_ = mli_HomTraComp_from_compact(homtra);

        ux_original = mli_Vec_init(1.0, 0.0, 0.0);
        ux_forth = mli_HomTraComp_pos(&homtra_, ux_original);
        CHECK_MARGIN(-1.0, ux_forth.x, 1e-12);
        CHECK_MARGIN(0.0, ux_forth.y, 1e-12);
        CHECK_MARGIN(133.7, ux_forth.z, 1e-12);
        ux_back = mli_HomTraComp_pos_inverse(&homtra_, ux_forth);
        CHECK(ux_original.x == ux_back.x);
        CHECK(ux_original.y == ux_back.y);
        CHECK(ux_original.z == ux_back.z);
}

CASE("trans_orientation_forth_and_back_only_rot_component_set")
{
        struct mli_HomTraComp homtra;
        struct mli_HomTra homtra_;
        struct mli_Vec ux_original, ux_forth, ux_back;

        homtra.translation = mli_Vec_init(0.0, 0.0, 0.0);
        homtra.rotation = mli_Quaternion_set_tait_bryan(0.0, MLI_MATH_PI, 0.0);
        homtra_ = mli_HomTraComp_from_compact(homtra);

        ux_original = mli_Vec_init(1.0, 0.0, 0.0);
        ux_forth = mli_HomTraComp_dir(&homtra_, ux_original);
        CHECK_MARGIN(-1.0, ux_forth.x, 1e-12);
        CHECK_MARGIN(0.0, ux_forth.y, 1e-12);
        CHECK_MARGIN(0.0, ux_forth.z, 1e-12);
        ux_back = mli_HomTraComp_dir_inverse(&homtra_, ux_forth);
        CHECK(ux_original.x == ux_back.x);
        CHECK(ux_original.y == ux_back.y);
        CHECK(ux_original.z == ux_back.z);
}
