/* Copyright 2019 Sebastian Achim Mueller                                     */

CASE("parse_correct_csv")
{
        char *text = "#comment\n#units\n0.0,1.0\n1.0,2.0\n";
        struct mliFunc func = mliFunc_init();
        CHECK(mliFunc_malloc_from_csv(&func, text));

        CHECK(func.num_points == 2);
        CHECK(func.x[0] == 0.0);
        CHECK(func.y[0] == 1.0);

        CHECK(func.x[1] == 1.0);
        CHECK(func.y[1] == 2.0);

        mliFunc_free(&func);
}

CASE("parse_empty_text")
{
        char *text = "";
        struct mliFunc func = mliFunc_init();
        CHECK(mliFunc_malloc_from_csv(&func, text));
        CHECK(func.num_points == 0);
        mliFunc_free(&func);
}

CASE("parse_empty_csv")
{
        char *text = "# only a comment line\n";
        struct mliFunc func = mliFunc_init();
        CHECK(mliFunc_malloc_from_csv(&func, text));
        CHECK(func.num_points == 0);
        mliFunc_free(&func);
}

CASE("parse_empty_csv_no_newline")
{
        char *text = "# only a comment line";
        struct mliFunc func = mliFunc_init();
        CHECK(mliFunc_malloc_from_csv(&func, text));
        CHECK(func.num_points == 0);
        mliFunc_free(&func);
}

CASE("some_comments")
{
        char *text = "0.0,1.0\n# inline comment\n1.0,2.0\n# end comment";
        struct mliFunc func = mliFunc_init();
        CHECK(mliFunc_malloc_from_csv(&func, text));

        CHECK(func.num_points == 2);
        CHECK(func.x[0] == 0.0);
        CHECK(func.y[0] == 1.0);

        CHECK(func.x[1] == 1.0);
        CHECK(func.y[1] == 2.0);

        mliFunc_free(&func);
}

CASE("exceed_buffer")
{
        char *text = "0.0000000000000000000000000000000000000000000"
        "000000000000000000000000000000000000000000000000000000000,"
        "1.00000000000000000000000000000000000000000000000000000000"
        "0000000000000000000000000000000000000000000000000000000000";
        struct mliFunc func = mliFunc_init();
        CHECK(0 == mliFunc_malloc_from_csv(&func, text));

        mliFunc_free(&func);
}

CASE("do_not_tolerate_whitespaces")
{
        struct mliFunc func = mliFunc_init();
        char text[128];

        sprintf(text, " 0.0,1.0\n");
        CHECK(0 == mliFunc_malloc_from_csv(&func, text));

        sprintf(text, "0.0 ,1.0\n");
        CHECK(0 == mliFunc_malloc_from_csv(&func, text));
        mliFunc_free(&func);

        sprintf(text, "0.0, 1.0\n");
        CHECK(0 == mliFunc_malloc_from_csv(&func, text));
        mliFunc_free(&func);

        sprintf(text, "0.0,1.0 \n");
        CHECK(0 == mliFunc_malloc_from_csv(&func, text));
        mliFunc_free(&func);

        sprintf(text, "\t0.0,1.0 \n");
        CHECK(0 == mliFunc_malloc_from_csv(&func, text));
        mliFunc_free(&func);
}
