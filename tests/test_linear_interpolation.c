/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("linear interpolation") {
    /*                                 xarg  x0  y0  x1  y1            */
    CHECK_MARGIN(mli_linear_interpolate(0.5, 0., 1., 1., 2.), 1.5, 1e-9);
    CHECK_MARGIN(mli_linear_interpolate(0.5, 0., 1., 1., 1.), 1., 1e-9);
    CHECK_MARGIN(mli_linear_interpolate(0.5, 0., 1., 1., 0.), .5, 1e-9);
}
