/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "mli_testing.h"
#include "mliColor.h"

CASE("mliColor_mix")
{
        struct mliColor red = {255., 0., 0.};
        struct mliColor blue = {0., 0., 255.};
        struct mliColor mix = mliColor_mix(red, blue, 0.2);
        CHECK_MARGIN(mix.r, 255. * 0.8, 1e-6);
        CHECK_MARGIN(mix.g, 0., 1e-6);
        CHECK_MARGIN(mix.b, 255. * 0.2, 1e-6);
}

CASE("mliColor_mean")
{
        struct mliColor colors[3];
        struct mliColor one = {10., 20., 30.};
        struct mliColor two = {1., 2., 3.};
        struct mliColor three = {50., 60., 70.};
        struct mliColor mean;
        colors[0] = one;
        colors[1] = two;
        colors[2] = three;
        mean = mliColor_mean(colors, 3);
        CHECK_MARGIN(mean.r, (10. + 1. + 50.) / 3., 1e-6);
        CHECK_MARGIN(mean.g, (20. + 2. + 60.) / 3., 1e-6);
        CHECK_MARGIN(mean.b, (30. + 3. + 70.) / 3., 1e-5);
}

CASE("mliColor 8bit range")
{
        struct mliColor c;
        c = mliColor_set(0.0, 0.0, 0.0);
        CHECK(mliColor_is_valid_8bit_range(c));

        c = mliColor_set(-MLI_EPSILON, 0.0, 0.0);
        CHECK(!mliColor_is_valid_8bit_range(c));

        c = mliColor_set(0.0, -MLI_EPSILON, 0.0);
        CHECK(!mliColor_is_valid_8bit_range(c));

        c = mliColor_set(0.0, 0.0, -MLI_EPSILON);
        CHECK(!mliColor_is_valid_8bit_range(c));

        c = mliColor_set(255.9, 255.9, 255.9);
        CHECK(mliColor_is_valid_8bit_range(c));

        c = mliColor_set(256.0, 255.9, 255.9);
        CHECK(!mliColor_is_valid_8bit_range(c));

        c = mliColor_set(255.9, 256.0, 255.9);
        CHECK(!mliColor_is_valid_8bit_range(c));

        c = mliColor_set(255.9, 255.9, 256.0);
        CHECK(!mliColor_is_valid_8bit_range(c));

        c = mliColor_set(1.0, MLI_NAN, 1.0);
        CHECK(!mliColor_is_valid_8bit_range(c));
}
