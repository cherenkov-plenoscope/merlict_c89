/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "vector_testing.h"

CASE("malloc, and free")
{
        uint64_t i;
        struct mtlDynDummy vec = mtlDynDummy_init();
        CHECK(mtlDynDummy_malloc(&vec, 0u));
        CHECK(vec.size == 0u);

        for (i = 0; i < 100; i++) {
                struct mtlDummy color;
                color.r = i * 1.;
                color.g = i * 2.;
                color.b = i * 3.;
                CHECK(vec.size == i);
                CHECK(mtlDynDummy_push_back(&vec, color));
                CHECK(vec.size == i + 1);
        }

        for (i = 0; i < vec.size; i++) {
                struct mtlDummy color = vec.array[i];
                CHECK_MARGIN(color.r, i * 1., 1e-9);
                CHECK_MARGIN(color.g, i * 2., 1e-9);
                CHECK_MARGIN(color.b, i * 3., 1e-9);
                CHECK(vec.size == 100);
        }

        mtlDynDummy_free(&vec);
        CHECK(vec.size == 0);
        CHECK(vec.capacity == 0);
}

CASE("with pointers to mtlDummy")
{
        uint64_t i;
        struct mtlDynDummyPtr vec = mtlDynDummyPtr_init();
        CHECK(mtlDynDummyPtr_malloc(&vec, 0u));
        CHECK(vec.size == 0u);

        for (i = 0; i < 10; i++) {
                struct mtlDummy *ptr_color_in = (struct mtlDummy *)(i * i);
                CHECK(vec.size == i);
                CHECK(mtlDynDummyPtr_push_back(&vec, ptr_color_in));
                CHECK(vec.size == i + 1);
        }

        for (i = 0; i < vec.size; i++) {
                struct mtlDummy *ptr_color_out = vec.array[i];
                CHECK(ptr_color_out == (struct mtlDummy *)(i * i));
                CHECK(vec.size == 10);
        }

        mtlDynDummyPtr_free(&vec);
        CHECK(vec.size == 0);
        CHECK(vec.capacity == 0);
}

CASE("DynArray")
{
        struct mtlDummy c;
        struct mtlDynDummy colors = mtlDynDummy_init();
        CHECK(mtlDynDummy_test_init(&colors));

        CHECK(mtlDynDummy_malloc(&colors, 10));
        CHECK(mtlDynDummy_test_malloc(&colors, 10));

        colors.array[0].r = 4.0;
        colors.array[0].g = 5.9;
        colors.array[0].b = 2.9;
        c = colors.array[0];
        CHECK_MARGIN(c.r, 4.0, 1e-6);
        CHECK_MARGIN(c.g, 5.9, 1e-6);
        CHECK_MARGIN(c.b, 2.9, 1e-6);

        mtlDynDummy_free(&colors);
        CHECK(mtlDynDummy_test_free(&colors));
}

CASE("DynArray malloc 0")
{
        struct mtlDynDummy colors = mtlDynDummy_init();
        CHECK(mtlDynDummy_test_init(&colors));

        CHECK(mtlDynDummy_malloc(&colors, 0));
        CHECK(mtlDynDummy_test_malloc(&colors, 0));

        mtlDynDummy_free(&colors);
        CHECK(mtlDynDummy_test_free(&colors));
}

CASE("DynArray push")
{
        uint64_t i;
        struct mtlDynDummy channel = mtlDynDummy_init();
        CHECK(mtlDynDummy_test_init(&channel));

        CHECK(mtlDynDummy_malloc(&channel, 0));
        CHECK(mtlDynDummy_test_malloc(&channel, 0));

        for (i = 0; i < 1337 * 1337; i++) {
                struct mtlDummy c;
                c.r = 1. * (float)i;
                c.g = 2. * (float)i;
                c.b = -1. * (float)i;
                CHECK(mtlDynDummy_push_back(&channel, c));
        }
        CHECK(channel.size == 1337 * 1337);
        CHECK(channel.array != NULL);
        CHECK(channel.capacity >= channel.size);

        for (i = 0; i < channel.size; i++) {
                CHECK_MARGIN(channel.array[i].r, 1. * (float)i, 1e-1);
                CHECK_MARGIN(channel.array[i].g, 2. * (float)i, 1e-1);
                CHECK_MARGIN(channel.array[i].b, -1. * (float)i, 1e-1);
        }
        mtlDynDummy_free(&channel);
        CHECK(mtlDynDummy_test_free(&channel));

        CHECK(mtlDynDummy_malloc(&channel, 100));
        CHECK(mtlDynDummy_test_malloc(&channel, 100));
        mtlDynDummy_free(&channel);
        CHECK(mtlDynDummy_test_free(&channel));
}
