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

CASE("mli_Func_fold_numeric")
{
        double aa, ab, ba, bb, integral_0_1_x_square;
        struct mli_Func a, b;
        a = mli_Func_init();
        b = mli_Func_init();
        CHECK(mli_Func_malloc(&a, 6u));
        CHECK(mli_Func_malloc(&b, 6u));

        integral_0_1_x_square = 1. / 3.;

        /*
         *  a
         *           ___
         *          |   \
         *          |    \
         *          |     \
         *          |      \
         *      |---|---|---|---|
         *      0   1   2   3   4
         *  b
         *               ___
         *              /   |
         *             /    |
         *            /     |
         *           /      |
         *      |---|---|---|---|
         *      0   1   2   3   4
         */

        a.x[0] = 0.;
        a.y[0] = 0.;
        a.x[1] = 1.;
        a.y[1] = 0.;
        a.x[2] = 1 + 1e-6;
        a.y[2] = 1.;
        a.x[3] = 2.;
        a.y[3] = 1.;
        a.x[4] = 3.;
        a.y[4] = 0.;
        a.x[5] = 4.;
        a.y[5] = 0.;

        b.x[0] = 0.;
        b.y[0] = 0.;
        b.x[1] = 1.;
        b.y[1] = 0.;
        b.x[2] = 2.;
        b.y[2] = 1.;
        b.x[3] = 3.;
        b.y[3] = 1.;
        b.x[4] = 3 + 1e-6;
        b.y[4] = 0.;
        b.x[5] = 4.;
        b.y[5] = 0.;

        CHECK(mli_Func_fold_numeric(&a, &a, &aa));
        CHECK_MARGIN(aa, 1.0 + integral_0_1_x_square, 1e-3);
        CHECK(mli_Func_fold_numeric(&a, &b, &ab));
        CHECK_MARGIN(ab, 1.0, 1e-3);
        CHECK(mli_Func_fold_numeric(&b, &a, &ba));
        CHECK_MARGIN(ab, ba, 1e-3);
        CHECK(mli_Func_fold_numeric(&b, &b, &bb));
        CHECK_MARGIN(bb, aa, 1e-3);

        mli_Func_free(&a);
        mli_Func_free(&b);
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
