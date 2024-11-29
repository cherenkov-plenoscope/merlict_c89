/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "../../mli_testing/src/mli_testing.h"

CASE("init")
{
        struct mtlArrayChar arr = mtlArrayChar_init();
        CHECK(arr.size == 0);
        CHECK(arr.array == NULL);
}

CASE("malloc_realloc_free")
{
        struct mtlArrayChar arr = mtlArrayChar_init();
        CHECK(mtlArrayChar_malloc(&arr, 12u));
        CHECK(arr.size == 12u);
        CHECK(arr.array != NULL);

        mtlArrayChar_free(&arr);
        CHECK(arr.size == 0);
        CHECK(arr.array == NULL);
}

CASE("realloc")
{
        uint64_t i;
        struct mtlArrayChar arr = mtlArrayChar_init();
        CHECK(mtlArrayChar_malloc(&arr, 12u));

        for (i = 0; i < arr.size; i++) {
                CHECK(mtlArrayChar_set(&arr, i, i));
        }

        CHECK(mtlArrayChar_realloc(&arr, 6u));
        CHECK(arr.size == 6u);
        for (i = 0; i < arr.size; i++) {
                char v;
                CHECK(mtlArrayChar_get(&arr, i, &v));
                CHECK(v == (char)i);
        }

        mtlArrayChar_free(&arr);
        CHECK(arr.size == 0);
        CHECK(arr.array == NULL);
}

CASE("out_of_range")
{
        char v = 25;
        struct mtlArrayChar arr = mtlArrayChar_init();
        CHECK(mtlArrayChar_malloc(&arr, 7u));

        CHECK(!mtlArrayChar_set(&arr, 7u, v));
        CHECK(!mtlArrayChar_get(&arr, 7u, &v));

        mtlArrayChar_free(&arr);
}
