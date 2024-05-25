/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "../../mli_testing/src/mli_testing.h"
#include "../src/mliAvlDict.h"

CASE("mliAvlDict_basics")
{
        struct mliAvlDict dict = mliAvlDict_init();
        CHECK(mliAvlDict_malloc(&dict, 100));
        mliAvlDict_free(&dict);
}


CASE("mliAvlDict_fill")
{
        struct mliAvlDict dict = mliAvlDict_init();
        int64_t val;
        CHECK(mliAvlDict_malloc(&dict, 100));

        CHECK(dict.len == 0);
        CHECK(!mliAvlDict_has(&dict, 42));
        CHECK(mliAvlDict_set(&dict, 42, 420));
        CHECK(dict.len == 1);
        CHECK(mliAvlDict_has(&dict, 42));
        CHECK(mliAvlDict_get(&dict, 42, &val));
        CHECK(val == 420);
        CHECK(mliAvlDict_pop(&dict, 42));
        CHECK(dict.len == 0);
        CHECK(!mliAvlDict_has(&dict, 42));

        mliAvlDict_free(&dict);
}


CASE("mliAvlDict_hammer")
{
        int64_t back;
        int64_t i;

        struct mliAvlDict dict = mliAvlDict_init();

        CHECK(mliAvlDict_malloc(&dict, 10));

        for (i = 0; i < 100; i++) {
                CHECK(mliAvlDict_set(&dict, i, 2*i));
        }

        CHECK(dict.len == 100);
        CHECK(dict.back == 100);
        CHECK(dict.capacity >= dict.back);

        for (i = 0; i < 100; i++) {
                CHECK(mliAvlDict_get(&dict, i, &back));
                CHECK(2*i == back);
        }

        for (i = 800; i < 900; i++) {
                CHECK(!mliAvlDict_has(&dict, i));
        }

        for (i = 0; i < 100; i++) {
                CHECK(mliAvlDict_has(&dict, i));
        }

        for (i = 0; i < 100; i++) {
                CHECK(mliAvlDict_pop(&dict, i));
        }
        CHECK(dict.len == 0);
        CHECK(dict.capacity >= dict.back);

        mliAvlDict_free(&dict);
}
