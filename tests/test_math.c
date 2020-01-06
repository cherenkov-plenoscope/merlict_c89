/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("rad2deg, deg2rad") {
    int i;
    for (i = -721; i < 721; i++) {
        double angle_in_deg = (double)i;
        CHECK_MARGIN(
            angle_in_deg,
            mli_rad2deg(mli_deg2rad(angle_in_deg)),
            1e-9);
    }
}

CASE("deg2rad, explicit") {
    CHECK_MARGIN(mli_deg2rad(0.), 0., 1e-9);
    CHECK_MARGIN(mli_deg2rad(90.), MLI_PI/2, 1e-9);
    CHECK_MARGIN(mli_deg2rad(180.), MLI_PI, 1e-9);
    CHECK_MARGIN(mli_deg2rad(-90.), -MLI_PI/2.0, 1e-9);
    CHECK_MARGIN(mli_deg2rad(-180.), -MLI_PI, 1e-9);
    CHECK_MARGIN(mli_deg2rad(360.), 2.*MLI_PI, 1e-9);
}

CASE("rad2deg, explicit") {
    CHECK_MARGIN(mli_rad2deg(0.*MLI_PI), 0., 1e-9);
    CHECK_MARGIN(mli_rad2deg(.25*MLI_PI), 45., 1e-9);
    CHECK_MARGIN(mli_rad2deg(.5*MLI_PI), 90., 1e-9);
    CHECK_MARGIN(mli_rad2deg(1.*MLI_PI), 180., 1e-9);
    CHECK_MARGIN(mli_rad2deg(1.e3*MLI_PI), 180.e3, 1e-9);
}

CASE("MAX2") {
    CHECK(MLI_MAX2(3, 4) == 4);
    CHECK(MLI_MAX2(4, 3) == 4);
}

CASE("MIN3") {
    CHECK(MLI_MIN3(4, 5, 6) == 4);
    CHECK(MLI_MIN3(5, 4, 6) == 4);
    CHECK(MLI_MIN3(5, 6, 4) == 4);
}

CASE("MAX3") {
    CHECK(MLI_MAX3(4, 5, 6) == 6);
    CHECK(MLI_MAX3(5, 4, 6) == 6);
    CHECK(MLI_MAX3(5, 6, 4) == 6);

    CHECK(MLI_MAX3(4., 5., 6.) == 6.);
    CHECK(MLI_MAX3(5., 4., 6.) == 6.);
    CHECK(MLI_MAX3(5., 6., 4.) == 6.);
}