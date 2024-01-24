/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "../../mli_testing/src/mli_testing.h"
#include "../src/mli_math.h"

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
        CHECK(MLI_ROUND(f) == 0.0);
        f = 0.1;
        CHECK(MLI_ROUND(f) == 0.0);
        f = 0.5 - 1e-6;
        CHECK(MLI_ROUND(f) == 0.0);
        f = 0.5 + 1e-6;
        CHECK(MLI_ROUND(f) == 1.0);
        f = 0.99;
        CHECK(MLI_ROUND(f) == 1.0);
        f = 1.0;
        CHECK(MLI_ROUND(f) == 1.0);
        f = -0.5 - 1e-9;
        CHECK(MLI_ROUND(f) == -1.0);
        f = -0.4;
        CHECK_MARGIN(MLI_ROUND(f), 0.0, 1e-9);
}

CASE("round to int")
{
        double f;

        f = 0.;
        CHECK(MLI_NEAR_INT(f) == 0);
        f = 0.1;
        CHECK(MLI_NEAR_INT(f) == 0);
        f = 0.5 - 1e-6;
        CHECK(MLI_NEAR_INT(f) == 0);
        f = 0.5 + 1e-6;
        CHECK(MLI_NEAR_INT(f) == 1);
        f = 0.99;
        CHECK(MLI_NEAR_INT(f) == 1);
        f = 1.0;
        CHECK(MLI_NEAR_INT(f) == 1);
        f = -0.5 - 1e-9;
        CHECK(MLI_NEAR_INT(f) == -1);
        f = -0.4;
        CHECK(MLI_NEAR_INT(f) == 0);
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

CASE("linear interpolation 1d")
{
        /*                                    xarg  x0  x1                    */
        CHECK_MARGIN(mli_linear_interpolate_1d(0.0, 0., 1.), 0.0, 1e-9);
        CHECK_MARGIN(mli_linear_interpolate_1d(0.5, 0., 1.), 0.5, 1e-9);
        CHECK_MARGIN(mli_linear_interpolate_1d(1.0, 0., 1.), 1.0, 1e-9);

        CHECK_MARGIN(mli_linear_interpolate_1d(0.0, 2., 3.), 2.0, 1e-9);
        CHECK_MARGIN(mli_linear_interpolate_1d(0.5, 2., 3.), 2.5, 1e-9);
        CHECK_MARGIN(mli_linear_interpolate_1d(1.0, 2., 3.), 3.0, 1e-9);
}

CASE("linear interpolation 2d")
{
        /*                                    xarg  x0  y0  x1  y1            */
        CHECK_MARGIN(mli_linear_interpolate_2d(0.5, 0., 1., 1., 2.), 1.5, 1e-9);
        CHECK_MARGIN(mli_linear_interpolate_2d(0.5, 0., 1., 1., 1.), 1., 1e-9);
        CHECK_MARGIN(mli_linear_interpolate_2d(0.5, 0., 1., 1., 0.), .5, 1e-9);
}

CASE("upper_compare_double, 3 points")
{
        double points[3] = {0., 1., 2.};
        uint64_t num_points = 3;
        CHECK(mli_upper_compare_double(points, num_points, -1.) == 0);
        CHECK(mli_upper_compare_double(points, num_points, 3.) == 3);
}

CASE("upper_compare_double, 0 points")
{
        double *points = NULL;
        uint64_t num_points = 0;
        CHECK(mli_upper_compare_double(points, num_points, 4.5) == 0);
}

CASE("upper_compare_double, ascending")
{
        double points[8] = {1., 2., 3., 4., 5., 6., 7., 8.};
        uint64_t num_points = 8;
        CHECK(mli_upper_compare_double(points, num_points, 4.5) == 4);
}

CASE("upper_compare_double, past last item")
{
        double points[8] = {1., 2., 3., 4., 5., 6., 7., 8.};
        uint64_t num_points = 3;
        CHECK(mli_upper_compare_double(points, num_points, 9.) == 3);
}

CASE("upper_compare_double, first item")
{
        double points[8] = {1., 2., 3., 4., 5., 6., 7., 8.};
        uint64_t num_points = 8;
        CHECK(mli_upper_compare_double(points, num_points, 0.) == 0);
}

CASE("upper_compare_double, in range item, 8")
{
        double points[8] = {1., 2., 3., 4., 5., 6., 7., 8.};
        uint64_t num_points = 8;
        CHECK(mli_upper_compare_double(points, num_points, 2.) == 2);
}

CASE("upper_compare_double, in range item, 3")
{
        double points[3] = {1., 2., 3.};
        uint64_t num_points = 3;
        CHECK(mli_upper_compare_double(points, num_points, 2.5) == 2);
}

CASE("histogram 1D")
{
        const double bin_edges[3] = {1., 2., 3.};
        const uint64_t num_bin_edges = 3;
        uint64_t bins[2] = {0u, 0u};
        uint64_t overflow_bin, underflow_bin;
        overflow_bin = 0u;
        underflow_bin = 0u;
        mli_histogram(
                bin_edges,
                num_bin_edges,
                &underflow_bin,
                bins,
                &overflow_bin,
                2.5);
        CHECK(underflow_bin == 0u);
        CHECK(bins[0] == 0u);
        CHECK(bins[1] == 1u);
        CHECK(overflow_bin == 0u);

        mli_histogram(
                bin_edges,
                num_bin_edges,
                &underflow_bin,
                bins,
                &overflow_bin,
                0.5);
        CHECK(underflow_bin == 1u);
        CHECK(bins[0] == 0u);
        CHECK(bins[1] == 1u);
        CHECK(overflow_bin == 0u);

        mli_histogram(
                bin_edges,
                num_bin_edges,
                &underflow_bin,
                bins,
                &overflow_bin,
                3.5);
        CHECK(underflow_bin == 1u);
        CHECK(bins[0] == 0u);
        CHECK(bins[1] == 1u);
        CHECK(overflow_bin == 1u);

        mli_histogram(
                bin_edges,
                num_bin_edges,
                &underflow_bin,
                bins,
                &overflow_bin,
                1.0);
        CHECK(underflow_bin == 1u);
        CHECK(bins[0] == 1u);
        CHECK(bins[1] == 1u);
        CHECK(overflow_bin == 1u);

        mli_histogram(
                bin_edges,
                num_bin_edges,
                &underflow_bin,
                bins,
                &overflow_bin,
                1.9);
        CHECK(underflow_bin == 1u);
        CHECK(bins[0] == 2u);
        CHECK(bins[1] == 1u);
        CHECK(overflow_bin == 1u);

        mli_histogram(
                bin_edges,
                num_bin_edges,
                &underflow_bin,
                bins,
                &overflow_bin,
                2.0);
        CHECK(underflow_bin == 1u);
        CHECK(bins[0] == 2u);
        CHECK(bins[1] == 2u);
        CHECK(overflow_bin == 1u);

        mli_histogram(
                bin_edges,
                num_bin_edges,
                &underflow_bin,
                bins,
                &overflow_bin,
                3.0);
        CHECK(underflow_bin == 1u);
        CHECK(bins[0] == 2u);
        CHECK(bins[1] == 2u);
        CHECK(overflow_bin == 2u);
}

CASE("mli_bin_edges_linspace")
{
        double bin_edges[3];
        const uint64_t num_bin_edges = 3;
        mli_linspace(0., 1., bin_edges, num_bin_edges);
        CHECK_MARGIN(bin_edges[0], 0., 1e-6);
        CHECK_MARGIN(bin_edges[1], 0.5, 1e-6);
        CHECK_MARGIN(bin_edges[2], 1., 1e-6);
}
