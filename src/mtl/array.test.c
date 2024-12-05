/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("init")
{
        struct mtlArrayfloat arr = mtlArrayfloat_init();
        CHECK(arr.size == 0);
        CHECK(arr.array == NULL);
}

CASE("malloc_realloc_free")
{
        struct mtlArrayfloat arr = mtlArrayfloat_init();
        CHECK(mtlArrayfloat_malloc(&arr, 12u));
        CHECK(arr.size == 12u);
        CHECK(arr.array != NULL);

        mtlArrayfloat_free(&arr);
        CHECK(arr.size == 0);
        CHECK(arr.array == NULL);
}

CASE("realloc")
{
        uint64_t i;
        struct mtlArrayfloat arr = mtlArrayfloat_init();
        CHECK(mtlArrayfloat_malloc(&arr, 12u));

        for (i = 0; i < arr.size; i++) {
                CHECK(mtlArrayfloat_set(&arr, i, i));
        }

        CHECK(mtlArrayfloat_realloc(&arr, 6u));
        CHECK(arr.size == 6u);
        for (i = 0; i < arr.size; i++) {
                float v;
                CHECK(mtlArrayfloat_get(&arr, i, &v));
                CHECK(v == (float)i);
        }

        mtlArrayfloat_free(&arr);
        CHECK(arr.size == 0);
        CHECK(arr.array == NULL);
}

CASE("out_of_range")
{
        float v = 25;
        struct mtlArrayfloat arr = mtlArrayfloat_init();
        CHECK(mtlArrayfloat_malloc(&arr, 7u));

        CHECK(!mtlArrayfloat_set(&arr, 7u, v));
        CHECK(!mtlArrayfloat_get(&arr, 7u, &v));

        mtlArrayfloat_free(&arr);
}

CASE("zero_termination")
{
        struct mtlArrayChar str = mtlArrayChar_init();
        CHECK(mtlArrayChar_malloc(&str, 7u));

        CHECK(str.size == 7u);
        CHECK(str.array[7u] == '\0');

        mtlArrayChar_free(&str);
}

CASE("mtlArrayChar_malloc_copyn")
{
        struct mtlArrayChar src = mtlArrayChar_init();
        struct mtlArrayChar dst = mtlArrayChar_init();
        char txt[] = "123456789.json";

        CHECK(mtlArrayChar_malloc(&src, strlen(txt)));
        strcpy(src.array, txt);

        CHECK(mtlArrayChar_copyn(&dst, &src, 0, src.size));
        CHECK(0 == strcmp(dst.array, "123456789.json"));

        CHECK(!mtlArrayChar_copyn(&dst, &src, 0, src.size + 1));

        CHECK(mtlArrayChar_copyn(&dst, &src, 0, 3));
        CHECK(0 == strcmp(dst.array, "123"));

        CHECK(mtlArrayChar_copyn(&dst, &src, 8, 3));
        CHECK(0 == strcmp(dst.array, "9.j"));

        CHECK(mtlArrayChar_copyn(&dst, &src, 8, 0));
        CHECK(0 == strcmp(dst.array, ""));

        mtlArrayChar_free(&dst);
        mtlArrayChar_free(&src);
}
