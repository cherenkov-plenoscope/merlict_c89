/* Copyright 2019-2020 Sebastian Achim Mueller                                */

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
