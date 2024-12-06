/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mli_Color_mix")
{
        struct mli_Color red = {255., 0., 0.};
        struct mli_Color blue = {0., 0., 255.};
        struct mli_Color mix = mli_Color_mix(red, blue, 0.2);
        CHECK_MARGIN(mix.r, 255. * 0.8, 1e-6);
        CHECK_MARGIN(mix.g, 0., 1e-6);
        CHECK_MARGIN(mix.b, 255. * 0.2, 1e-6);
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
