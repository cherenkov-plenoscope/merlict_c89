/* Copyright 2019-2020 Sebastian Achim Mueller                                */


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
