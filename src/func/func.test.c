/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mli_Func_malloc")
{
        struct mli_Func func = mli_Func_init();
        CHECK(mli_Func_malloc(&func, 0u));
        CHECK(func.num_points == 0u);
        CHECK(mli_Func_x_is_strictly_increasing(&func));
        mli_Func_free(&func);
}

CASE("mli_Func_x_is_strictly_increasing")
{
        struct mli_Func func = mli_Func_init();
        CHECK(mli_Func_malloc(&func, 3u));
        CHECK(func.num_points == 3u);
        func.x[0] = 0.;
        func.x[1] = 1.;
        func.x[2] = 2.;
        CHECK(mli_Func_x_is_strictly_increasing(&func));
        mli_Func_free(&func);
}

CASE("mli_Func_evaluate, explicit")
{
        double y;
        struct mli_Func func = mli_Func_init();
        CHECK(mli_Func_malloc(&func, 5u));
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

        CHECK(mli_Func_x_is_strictly_increasing(&func));
        CHECK(mli_math_upper_compare_double(func.x, func.num_points, 1.5) == 2);
        CHECK(mli_Func_evaluate(&func, 1.5, &y));
        CHECK(y == 2.5);
        mli_Func_free(&func);
}

CASE("mli_Func_evaluate, loop")
{
        double x, y;
        struct mli_Func func = mli_Func_init();
        CHECK(mli_Func_malloc(&func, 2u));
        func.x[0] = 0.;
        func.x[1] = 1.;
        func.y[0] = 0.;
        func.y[1] = 1.;
        CHECK(mli_Func_x_is_strictly_increasing(&func));
        for (x = 0.; x < 1.; x = x + 1e-2) {
                CHECK(mli_Func_evaluate(&func, x, &y));
                CHECK_MARGIN(y, x, 1e-6);
        }
        mli_Func_free(&func);
}

CASE("mli_Func_in_range")
{
        struct mli_Func func = mli_Func_init();
        CHECK(mli_Func_malloc(&func, 2u));
        func.x[0] = -2.5;
        func.x[1] = 4.2;

        CHECK(!mli_Func_in_range(&func, -5.0));
        CHECK(!mli_Func_in_range(&func, -2.51));
        CHECK(mli_Func_in_range(&func, -2.5));
        CHECK(mli_Func_in_range(&func, 0.0));
        CHECK(mli_Func_in_range(&func, 4.19));
        CHECK(!mli_Func_in_range(&func, 4.2));
        CHECK(!mli_Func_in_range(&func, 133.7));

        mli_Func_free(&func);
}
