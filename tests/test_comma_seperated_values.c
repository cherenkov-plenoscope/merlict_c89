/* Copyright 2019 Sebastian Achim Mueller                                     */

CASE("parse_correct_csv")
{
        char text[128];
        struct mliFunc func = mliFunc_init();

        sprintf(text, "#comment\n#units\n0.0,1.0\n1.0,2.0\n");
        CHECK(mliFunc_malloc_from_string(&func, text));

        CHECK(func.num_points == 2);
        CHECK(func.x[0] == 0.0);
        CHECK(func.y[0] == 1.0);

        CHECK(func.x[1] == 1.0);
        CHECK(func.y[1] == 2.0);

        mliFunc_free(&func);
}

CASE("parse_empty_text")
{
        char text[128];
        struct mliFunc func = mliFunc_init();

        memset(text, '\0', 128);
        CHECK(mliFunc_malloc_from_string(&func, text));
        CHECK(func.num_points == 0);
        mliFunc_free(&func);
}

CASE("parse_empty_csv")
{
        char text[128];
        struct mliFunc func = mliFunc_init();

        sprintf(text, "# only a comment line\n");
        CHECK(mliFunc_malloc_from_string(&func, text));
        CHECK(func.num_points == 0);
        mliFunc_free(&func);
}

CASE("parse_empty_csv_no_newline")
{
        char text[128];
        struct mliFunc func = mliFunc_init();

        sprintf(text, "# only a comment line");
        CHECK(mliFunc_malloc_from_string(&func, text));
        CHECK(func.num_points == 0);
        mliFunc_free(&func);
}

CASE("some_comments")
{
        char text[128];
        struct mliFunc func = mliFunc_init();

        sprintf(text, "0.0,1.0\n# inline comment\n1.0,2.0\n# end comment");
        CHECK(mliFunc_malloc_from_string(&func, text));

        CHECK(func.num_points == 2);
        CHECK(func.x[0] == 0.0);
        CHECK(func.y[0] == 1.0);

        CHECK(func.x[1] == 1.0);
        CHECK(func.y[1] == 2.0);

        mliFunc_free(&func);
}

CASE("exceed_buffer")
{
        char text[1024];
        struct mliFunc func = mliFunc_init();

        sprintf(
                text,
                "0.0000000000000000000000000000000000000000000"
                "000000000000000000000000000000000000000000000000000000000,"
                "1.00000000000000000000000000000000000000000000000000000000"
                "0000000000000000000000000000000000000000000000000000000000"
        );
        CHECK(0 == mliFunc_malloc_from_string(&func, text));
        mliFunc_free(&func);
}

CASE("do_not_tolerate_whitespaces")
{
        char text[128];
        struct mliFunc func = mliFunc_init();

        sprintf(text, " 0.0,1.0\n");
        CHECK(0 == mliFunc_malloc_from_string(&func, text));

        sprintf(text, "0.0 ,1.0\n");
        CHECK(0 == mliFunc_malloc_from_string(&func, text));
        mliFunc_free(&func);

        sprintf(text, "0.0, 1.0\n");
        CHECK(0 == mliFunc_malloc_from_string(&func, text));
        mliFunc_free(&func);

        sprintf(text, "0.0,1.0 \n");
        CHECK(0 == mliFunc_malloc_from_string(&func, text));
        mliFunc_free(&func);

        sprintf(text, "\t0.0,1.0 \n");
        CHECK(0 == mliFunc_malloc_from_string(&func, text));
        mliFunc_free(&func);
}
