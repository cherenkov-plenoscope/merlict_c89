/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "mli_testing.h"
#include "mli_math.h"

CASE("rad2deg, deg2rad")
{
        int i;
        for (i = -721; i < 721; i++) {
                double angle_in_deg = (double)i;
                CHECK_MARGIN(
                        angle_in_deg,
                        mli_rad2deg(mli_deg2rad(angle_in_deg)),
                        1e-9);
        }
}

CASE("deg2rad, explicit")
{
        CHECK_MARGIN(mli_deg2rad(0.), 0., 1e-9);
        CHECK_MARGIN(mli_deg2rad(90.), MLI_PI / 2, 1e-9);
        CHECK_MARGIN(mli_deg2rad(180.), MLI_PI, 1e-9);
        CHECK_MARGIN(mli_deg2rad(-90.), -MLI_PI / 2.0, 1e-9);
        CHECK_MARGIN(mli_deg2rad(-180.), -MLI_PI, 1e-9);
        CHECK_MARGIN(mli_deg2rad(360.), 2. * MLI_PI, 1e-9);
}

CASE("rad2deg, explicit")
{
        CHECK_MARGIN(mli_rad2deg(0. * MLI_PI), 0., 1e-9);
        CHECK_MARGIN(mli_rad2deg(.25 * MLI_PI), 45., 1e-9);
        CHECK_MARGIN(mli_rad2deg(.5 * MLI_PI), 90., 1e-9);
        CHECK_MARGIN(mli_rad2deg(1. * MLI_PI), 180., 1e-9);
        CHECK_MARGIN(mli_rad2deg(1.e3 * MLI_PI), 180.e3, 1e-9);
}

CASE("MAX2")
{
        CHECK(MLI_MAX2(3, 4) == 4);
        CHECK(MLI_MAX2(4, 3) == 4);
}

CASE("MIN3")
{
        CHECK(MLI_MIN3(4, 5, 6) == 4);
        CHECK(MLI_MIN3(5, 4, 6) == 4);
        CHECK(MLI_MIN3(5, 6, 4) == 4);
}

CASE("MAX3")
{
        CHECK(MLI_MAX3(4, 5, 6) == 6);
        CHECK(MLI_MAX3(5, 4, 6) == 6);
        CHECK(MLI_MAX3(5, 6, 4) == 6);

        CHECK(MLI_MAX3(4., 5., 6.) == 6.);
        CHECK(MLI_MAX3(5., 4., 6.) == 6.);
        CHECK(MLI_MAX3(5., 6., 4.) == 6.);
}

CASE("NAN")
{
        float hans = MLI_NAN;
        float peter = 0.;
        CHECK(MLI_IS_NAN(hans));
        CHECK(!MLI_IS_NAN(peter));
}

CASE("round to float")
{
        double f;

        f = 0.;
        CHECK(mli_roundf(f) == 0.0);
        f = 0.1;
        CHECK(mli_roundf(f) == 0.0);
        f = 0.5 - 1e-6;
        CHECK(mli_roundf(f) == 0.0);
        f = 0.5 + 1e-6;
        CHECK(mli_roundf(f) == 1.0);
        f = 0.99;
        CHECK(mli_roundf(f) == 1.0);
        f = 1.0;
        CHECK(mli_roundf(f) == 1.0);
        f = -0.5 - 1e-9;
        CHECK(mli_roundf(f) == -1.0);
        f = -0.4;
        CHECK_MARGIN(mli_roundf(f), 0.0, 1e-9);
}

CASE("round to int")
{
        double f;

        f = 0.;
        CHECK(mli_near_int(f) == 0);
        f = 0.1;
        CHECK(mli_near_int(f) == 0);
        f = 0.5 - 1e-6;
        CHECK(mli_near_int(f) == 0);
        f = 0.5 + 1e-6;
        CHECK(mli_near_int(f) == 1);
        f = 0.99;
        CHECK(mli_near_int(f) == 1);
        f = 1.0;
        CHECK(mli_near_int(f) == 1);
        f = -0.5 - 1e-9;
        CHECK(mli_near_int(f) == -1);
        f = -0.4;
        CHECK(mli_near_int(f) == 0);
}

CASE("bin centers in linear space")
{
        double c;
        c = mli_bin_center_in_linear_space(0.0, 1.0, 10, 0);
        CHECK(c == 0.05);

        /*

        0.0       0.25      0.5       0.75      1.0
        |----0----|----1----|----2----|----3----|
             |         |         |         |
           0.125       0.275     0.625     0.875

        */
        c = mli_bin_center_in_linear_space(0.0, 1.0, 4, 0);
        CHECK_MARGIN(c, 0.125, 1e-6);
        c = mli_bin_center_in_linear_space(0.0, 1.0, 4, 1);
        CHECK_MARGIN(c, 0.375, 1e-6);
        c = mli_bin_center_in_linear_space(0.0, 1.0, 4, 2);
        CHECK_MARGIN(c, 0.625, 1e-6);
        c = mli_bin_center_in_linear_space(0.0, 1.0, 4, 3);
        CHECK_MARGIN(c, 0.875, 1e-6);
}

CASE("linspace")
{
        double arr[16];
        mli_linspace(-1.0, 2.0, arr, 16);
        CHECK(arr[0] == -1.0);
        CHECK(arr[15] == 2.0);
}
