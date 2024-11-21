/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "../../mli_testing/src/mli_testing.h"
#include "../src/mliArray.h"

CASE("init")
{
        struct mliArrayChar arr = mliArrayChar_init();
        CHECK(arr.size == 0);
        CHECK(arr.array == NULL);
}

CASE("malloc_realloc_free")
{
        struct mliArrayChar arr = mliArrayChar_init();
        CHECK(mliArrayChar_malloc(&arr, 12u));
        CHECK(arr.size == 12u);
        CHECK(arr.array != NULL);

        mliArrayChar_free(&arr);
        CHECK(arr.size == 0);
        CHECK(arr.array == NULL);
}

CASE("realloc")
{
        uint64_t i;
        struct mliArrayChar arr = mliArrayChar_init();
        CHECK(mliArrayChar_malloc(&arr, 12u));

        for (i = 0; i < arr.size; i++) {
                CHECK(mliArrayChar_set(&arr, i, i));
        }

        CHECK(mliArrayChar_realloc(&arr, 6u));
        CHECK(arr.size == 6u);
        for (i = 0; i < arr.size; i++) {
                char v;
                CHECK(mliArrayChar_get(&arr, i, &v));
                CHECK(v == (char)i);
        }

        mliArrayChar_free(&arr);
        CHECK(arr.size == 0);
        CHECK(arr.array == NULL);
}

CASE("out_of_range")
{
        char v = 25;
        struct mliArrayChar arr = mliArrayChar_init();
        CHECK(mliArrayChar_malloc(&arr, 7u));

        CHECK(!mliArrayChar_set(&arr, 7u, v));
        CHECK(!mliArrayChar_get(&arr, 7u, &v));

        mliArrayChar_free(&arr);
}
