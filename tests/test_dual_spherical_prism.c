/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mli_is_inside_dual_circle_prism") {
    CHECK(mli_is_inside_dual_circle_prism(0., 0., 1., 2.));

    CHECK(mli_is_inside_dual_circle_prism(0., 0.9, 1., 2.));
    CHECK(!mli_is_inside_dual_circle_prism(0., 1.1, 1., 2.));

    CHECK(mli_is_inside_dual_circle_prism(0., -0.9, 1., 2.));
    CHECK(!mli_is_inside_dual_circle_prism(0., -1.1, 1., 2.));

    CHECK(!mli_is_inside_dual_circle_prism(-.51, 0., 1., 2.));
    CHECK(mli_is_inside_dual_circle_prism(-.49, 0., 1., 2.));

    CHECK(!mli_is_inside_dual_circle_prism(.51, 0., 1., 2.));
    CHECK(mli_is_inside_dual_circle_prism(.49, 0., 1., 2.));

    /* special case -> circle, height == width */
    CHECK(mli_is_inside_dual_circle_prism(0., 0.49, 1., 1.));
    CHECK(!mli_is_inside_dual_circle_prism(0., 0.51, 1., 1.));

    CHECK(mli_is_inside_dual_circle_prism(0., -0.49, 1., 1.));
    CHECK(!mli_is_inside_dual_circle_prism(0., -.51, 1., 1.));

    CHECK(!mli_is_inside_dual_circle_prism(-.51, 0., 1., 1.));
    CHECK(mli_is_inside_dual_circle_prism(-.49, 0., 1., 1.));

    CHECK(!mli_is_inside_dual_circle_prism(.51, 0., 1., 1.));
    CHECK(mli_is_inside_dual_circle_prism(.49, 0., 1., 1.));
}
