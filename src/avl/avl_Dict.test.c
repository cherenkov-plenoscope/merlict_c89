/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mli_AvlDict_basics")
{
        struct mli_AvlDict dict = mli_AvlDict_init();
        CHECK(mli_AvlDict_malloc(&dict, 100));
        mli_AvlDict_free(&dict);
}

CASE("mli_AvlDict_fill")
{
        struct mli_AvlDict dict = mli_AvlDict_init();
        int64_t val;
        CHECK(mli_AvlDict_malloc(&dict, 100));

        CHECK(dict.len == 0);
        CHECK(!mli_AvlDict_has(&dict, 42));
        CHECK(mli_AvlDict_set(&dict, 42, 420));
        CHECK(dict.len == 1);
        CHECK(mli_AvlDict_has(&dict, 42));
        CHECK(mli_AvlDict_get(&dict, 42, &val));
        CHECK(val == 420);
        CHECK(mli_AvlDict_pop(&dict, 42));
        CHECK(dict.len == 0);
        CHECK(!mli_AvlDict_has(&dict, 42));

        mli_AvlDict_free(&dict);
}

CASE("mli_AvlDict_hammer")
{
        int64_t back;
        int64_t i;

        struct mli_AvlDict dict = mli_AvlDict_init();

        CHECK(mli_AvlDict_malloc(&dict, 10));

        for (i = 0; i < 100; i++) {
                CHECK(mli_AvlDict_set(&dict, i, 2 * i));
        }

        CHECK(dict.len == 100);
        CHECK(dict.back == 100);
        CHECK(dict.capacity >= dict.back);

        for (i = 0; i < 100; i++) {
                CHECK(mli_AvlDict_get(&dict, i, &back));
                CHECK(2 * i == back);
        }

        for (i = 800; i < 900; i++) {
                CHECK(!mli_AvlDict_has(&dict, i));
        }

        for (i = 0; i < 100; i++) {
                CHECK(mli_AvlDict_has(&dict, i));
        }

        for (i = 0; i < 100; i++) {
                CHECK(mli_AvlDict_pop(&dict, i));
        }
        CHECK(dict.len == 0);
        CHECK(dict.capacity >= dict.back);

        mli_AvlDict_free(&dict);
}
