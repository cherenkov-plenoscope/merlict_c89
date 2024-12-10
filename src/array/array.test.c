/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("init")
{
        struct mli_ArrayTestingFloat arr = mli_ArrayTestingFloat_init();
        CHECK(arr.size == 0);
        CHECK(arr.array == NULL);
}

CASE("malloc_realloc_free")
{
        struct mli_ArrayTestingFloat arr = mli_ArrayTestingFloat_init();
        CHECK(mli_ArrayTestingFloat_malloc(&arr, 12u));
        CHECK(arr.size == 12u);
        CHECK(arr.array != NULL);

        mli_ArrayTestingFloat_free(&arr);
        CHECK(arr.size == 0);
        CHECK(arr.array == NULL);
}

CASE("realloc")
{
        uint64_t i;
        struct mli_ArrayTestingFloat arr = mli_ArrayTestingFloat_init();
        CHECK(mli_ArrayTestingFloat_malloc(&arr, 12u));

        for (i = 0; i < arr.size; i++) {
                CHECK(mli_ArrayTestingFloat_set(&arr, i, i));
        }

        CHECK(mli_ArrayTestingFloat_realloc(&arr, 6u));
        CHECK(arr.size == 6u);
        for (i = 0; i < arr.size; i++) {
                float v;
                CHECK(mli_ArrayTestingFloat_get(&arr, i, &v));
                CHECK(v == (float)i);
        }

        mli_ArrayTestingFloat_free(&arr);
        CHECK(arr.size == 0);
        CHECK(arr.array == NULL);
}

CASE("out_of_range")
{
        float v = 25;
        struct mli_ArrayTestingFloat arr = mli_ArrayTestingFloat_init();
        CHECK(mli_ArrayTestingFloat_malloc(&arr, 7u));

        CHECK(!mli_ArrayTestingFloat_set(&arr, 7u, v));
        CHECK(!mli_ArrayTestingFloat_get(&arr, 7u, &v));

        mli_ArrayTestingFloat_free(&arr);
}

CASE("zero_termination")
{
        struct mli_ArrayTestingChar str = mli_ArrayTestingChar_init();
        CHECK(mli_ArrayTestingChar_malloc(&str, 7u));

        CHECK(str.size == 7u);
        CHECK(str.array[7u] == '\0');

        mli_ArrayTestingChar_free(&str);
}

CASE("mli_ArrayTestingChar_malloc_copyn")
{
        struct mli_ArrayTestingChar src = mli_ArrayTestingChar_init();
        struct mli_ArrayTestingChar dst = mli_ArrayTestingChar_init();
        char txt[] = "123456789.json";

        CHECK(mli_ArrayTestingChar_malloc(&src, strlen(txt)));
        strcpy(src.array, txt);

        CHECK(mli_ArrayTestingChar_copyn(&dst, &src, 0, src.size));
        CHECK(0 == strcmp(dst.array, "123456789.json"));

        CHECK(!mli_ArrayTestingChar_copyn(&dst, &src, 0, src.size + 1));

        CHECK(mli_ArrayTestingChar_copyn(&dst, &src, 0, 3));
        CHECK(0 == strcmp(dst.array, "123"));

        CHECK(mli_ArrayTestingChar_copyn(&dst, &src, 8, 3));
        CHECK(0 == strcmp(dst.array, "9.j"));

        CHECK(mli_ArrayTestingChar_copyn(&dst, &src, 8, 0));
        CHECK(0 == strcmp(dst.array, ""));

        mli_ArrayTestingChar_free(&dst);
        mli_ArrayTestingChar_free(&src);
}
