/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "mli_testing.h"
#include "mliFunc.h"

CASE("mliFunc_malloc")
{
        struct mliFunc func = mliFunc_init();
        CHECK(mliFunc_malloc(&func, 0u));
        CHECK(func.num_points == 0u);
        CHECK(mliFunc_x_is_strictly_increasing(&func));
        mliFunc_free(&func);
}

CASE("mliFunc_x_is_strictly_increasing")
{
        struct mliFunc func = mliFunc_init();
        CHECK(mliFunc_malloc(&func, 3u));
        CHECK(func.num_points == 3u);
        func.x[0] = 0.;
        func.x[1] = 1.;
        func.x[2] = 2.;
        CHECK(mliFunc_x_is_strictly_increasing(&func));
        mliFunc_free(&func);
}

CASE("mliFunc_evaluate, explicit")
{
        double y;
        struct mliFunc func = mliFunc_init();
        CHECK(mliFunc_malloc(&func, 5u));
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

        CHECK(mliFunc_x_is_strictly_increasing(&func));
        CHECK(mli_upper_compare_double(func.x, func.num_points, 1.5) == 2);
        CHECK(mliFunc_evaluate(&func, 1.5, &y));
        CHECK(y == 2.5);
        mliFunc_free(&func);
}

CASE("mliFunc_evaluate, loop")
{
        double x, y;
        struct mliFunc func = mliFunc_init();
        CHECK(mliFunc_malloc(&func, 2u));
        func.x[0] = 0.;
        func.x[1] = 1.;
        func.y[0] = 0.;
        func.y[1] = 1.;
        CHECK(mliFunc_x_is_strictly_increasing(&func));
        for (x = 0.; x < 1.; x = x + 1e-2) {
                CHECK(mliFunc_evaluate(&func, x, &y));
                CHECK_MARGIN(y, x, 1e-6);
        }
        mliFunc_free(&func);
}

CASE("mliFunc_fold_numeric")
{
        double aa, ab, ba, bb, integral_0_1_x_square;
        struct mliFunc a, b;
        a = mliFunc_init();
        b = mliFunc_init();
        CHECK(mliFunc_malloc(&a, 6u));
        CHECK(mliFunc_malloc(&b, 6u));

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

        CHECK(mliFunc_fold_numeric(&a, &a, &aa));
        CHECK_MARGIN(aa, 1.0 + integral_0_1_x_square, 1e-3);
        CHECK(mliFunc_fold_numeric(&a, &b, &ab));
        CHECK_MARGIN(ab, 1.0, 1e-3);
        CHECK(mliFunc_fold_numeric(&b, &a, &ba));
        CHECK_MARGIN(ab, ba, 1e-3);
        CHECK(mliFunc_fold_numeric(&b, &b, &bb));
        CHECK_MARGIN(bb, aa, 1e-3);

        mliFunc_free(&a);
        mliFunc_free(&b);
}

CASE("mliFunc_json")
{
        uint64_t token = 0;
        uint64_t token_f = 0;
        struct mliFunc f = mliFunc_init();
        struct mliJson json = mliJson_init();
        char json_str[] = "{\"function\": [[0, 10], [1, 11], [2, 12]]}";
        CHECK(mliJson_malloc_from_string(&json, json_str));

        CHECK(mliJson_find_key(&json, token, "function", &token_f));

        CHECK(mliFunc_malloc_from_json_token(&f, &json, token_f + 1));

        CHECK(f.num_points == 3);
        CHECK(f.x[0] == 0.0);
        CHECK(f.y[0] == 10.0);
        CHECK(f.x[1] == 1.0);
        CHECK(f.y[1] == 11.0);
        CHECK(f.x[2] == 2.0);
        CHECK(f.y[2] == 12.0);
        mliJson_free(&json);
}
