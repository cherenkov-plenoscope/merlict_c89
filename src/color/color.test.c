/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mli_Color_truncate")
{
        struct mli_Color in = mli_Color_set(100.0, -14.0, 16.0);
        struct mli_Color out = mli_Color_truncate(in, -6.0, 24.0);
        CHECK_MARGIN(out.r, 24.0, 1e-6);
        CHECK_MARGIN(out.g, -6.0, 1e-6);
        CHECK_MARGIN(out.b, 16.0, 1e-6);
}

CASE("mli_Color_mean")
{
        struct mli_Color colors[3];
        struct mli_Color one = {10., 20., 30.};
        struct mli_Color two = {1., 2., 3.};
        struct mli_Color three = {50., 60., 70.};
        struct mli_Color mean;
        colors[0] = one;
        colors[1] = two;
        colors[2] = three;
        mean = mli_Color_mean(colors, 3);
        CHECK_MARGIN(mean.r, (10. + 1. + 50.) / 3., 1e-6);
        CHECK_MARGIN(mean.g, (20. + 2. + 60.) / 3., 1e-6);
        CHECK_MARGIN(mean.b, (30. + 3. + 70.) / 3., 1e-5);
}

CASE("mli_Color 8bit range")
{
        struct mli_Color c;
        c = mli_Color_set(0.0, 0.0, 0.0);
        CHECK(mli_Color_is_in_range(c, 0., 256.));

        c = mli_Color_set(-MLI_MATH_EPSILON, 0.0, 0.0);
        CHECK(!mli_Color_is_in_range(c, 0., 256.));

        c = mli_Color_set(0.0, -MLI_MATH_EPSILON, 0.0);
        CHECK(!mli_Color_is_in_range(c, 0., 256.));

        c = mli_Color_set(0.0, 0.0, -MLI_MATH_EPSILON);
        CHECK(!mli_Color_is_in_range(c, 0., 256.));

        c = mli_Color_set(255.9, 255.9, 255.9);
        CHECK(mli_Color_is_in_range(c, 0., 256.));

        c = mli_Color_set(256.0, 255.9, 255.9);
        CHECK(!mli_Color_is_in_range(c, 0., 256.));

        c = mli_Color_set(255.9, 256.0, 255.9);
        CHECK(!mli_Color_is_in_range(c, 0., 256.));

        c = mli_Color_set(255.9, 255.9, 256.0);
        CHECK(!mli_Color_is_in_range(c, 0., 256.));

        c = mli_Color_set(1.0, MLI_MATH_NAN, 1.0);
        CHECK(!mli_Color_is_in_range(c, 0., 256.));
}
