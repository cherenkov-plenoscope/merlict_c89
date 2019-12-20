/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mliFunc_malloc") {
    mliFunc func = mliFunc_init();
    func.num_points = 0u;
    CHECK(mliFunc_malloc(&func));
    CHECK(func.num_points == 0u);
    CHECK(mliFunc_x_is_causal(&func));
    mliFunc_free(&func);
}

CASE("mliFunc_x_is_causal") {
    mliFunc func = mliFunc_init();
    func.num_points = 3u;
    CHECK(mliFunc_malloc(&func));
    CHECK(func.num_points == 3u);
    func.x[0] = 0.;
    func.x[1] = 1.;
    func.x[2] = 2.;
    CHECK(mliFunc_x_is_causal(&func));
    mliFunc_free(&func);
}

CASE("mliFunc_evaluate, explicit") {
    double y;
    mliFunc func = mliFunc_init();
    func.num_points = 5u;
    CHECK(mliFunc_malloc(&func));
    CHECK(func.num_points == 5u);

    func.x[0] = 0.;
    func.x[1] = 1.;
    func.x[2] = 2.;
    func.x[3] = 3.;
    func.x[4] = 4.;

    func.y[0] = 4.;
    func.y[1] = 3.;
    func.y[2] = 2.;
    func.y[3] = 1.;
    func.y[4] = 0.;

    CHECK(mliFunc_x_is_causal(&func));
    CHECK(mli_upper_compare_double(func.x, func.num_points, 1.5) == 2);
    CHECK(mliFunc_evaluate(&func, 1.5, &y));
    CHECK(y == 2.5);
    mliFunc_free(&func);
}

CASE("mliFunc_evaluate, loop") {
    double x, y;
    mliFunc func = mliFunc_init();
    func.num_points = 2u;
    CHECK(mliFunc_malloc(&func));
    func.x[0] = 0.;
    func.x[1] = 1.;
    func.y[0] = 0.;
    func.y[1] = 1.;
    CHECK(mliFunc_x_is_causal(&func));
    for (x = 0.; x < 1.; x = x + 1e-2) {
        CHECK(mliFunc_evaluate(&func, x, &y));
        CHECK_MARGIN(y, x, 1e-6);}
    mliFunc_free(&func);
}
