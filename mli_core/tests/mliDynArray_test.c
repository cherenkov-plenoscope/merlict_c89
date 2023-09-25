/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "../../mli_testing/include/mli_testing.h"
#include "../src/mliDynArray.h"
#include "../src/mliDynArray_testing.h"

CASE("malloc, and free")
{
        uint64_t i;
        struct mliDynColor vec = mliDynColor_init();
        CHECK(mliDynColor_malloc(&vec, 0u));
        CHECK(vec.size == 0u);

        for (i = 0; i < 100; i++) {
                struct mliColor color = mliColor_set(i * 1., i * 2., i * 3.);
                CHECK(vec.size == i);
                CHECK(mliDynColor_push_back(&vec, color));
                CHECK(vec.size == i + 1);
        }

        for (i = 0; i < vec.size; i++) {
                struct mliColor color = vec.array[i];
                CHECK_MARGIN(color.r, i * 1., 1e-9);
                CHECK_MARGIN(color.g, i * 2., 1e-9);
                CHECK_MARGIN(color.b, i * 3., 1e-9);
                CHECK(vec.size == 100);
        }

        mliDynColor_free(&vec);
        CHECK(vec.size == 0);
        CHECK(vec.capacity == 0);
}

CASE("with pointers to mliColor")
{
        uint64_t i;
        struct mliDynColorPtr vec = mliDynColorPtr_init();
        CHECK(mliDynColorPtr_malloc(&vec, 0u));
        CHECK(vec.size == 0u);

        for (i = 0; i < 10; i++) {
                struct mliColor *ptr_color_in = (struct mliColor *)(i * i);
                CHECK(vec.size == i);
                CHECK(mliDynColorPtr_push_back(&vec, ptr_color_in));
                CHECK(vec.size == i + 1);
        }

        for (i = 0; i < vec.size; i++) {
                struct mliColor *ptr_color_out = vec.array[i];
                CHECK(ptr_color_out == (struct mliColor *)(i * i));
                CHECK(vec.size == 10);
        }

        mliDynColorPtr_free(&vec);
        CHECK(vec.size == 0);
        CHECK(vec.capacity == 0);
}

CASE("DynArray")
{
        struct mliColor c;
        struct mliDynColor colors = mliDynColor_init();
        CHECK(mliDynColor_test_init(&colors));

        CHECK(mliDynColor_malloc(&colors, 10));
        CHECK(mliDynColor_test_malloc(&colors, 10));

        colors.array[0].r = 4.0;
        colors.array[0].g = 5.9;
        colors.array[0].b = 2.9;
        c = colors.array[0];
        CHECK_MARGIN(c.r, 4.0, 1e-6);
        CHECK_MARGIN(c.g, 5.9, 1e-6);
        CHECK_MARGIN(c.b, 2.9, 1e-6);

        mliDynColor_free(&colors);
        CHECK(mliDynColor_test_free(&colors));
}

CASE("DynArray malloc 0")
{
        struct mliDynColor colors = mliDynColor_init();
        CHECK(mliDynColor_test_init(&colors));

        CHECK(mliDynColor_malloc(&colors, 0));
        CHECK(mliDynColor_test_malloc(&colors, 0));

        mliDynColor_free(&colors);
        CHECK(mliDynColor_test_free(&colors));
}

CASE("DynArray push")
{
        uint64_t i;
        struct mliDynColor channel = mliDynColor_init();
        CHECK(mliDynColor_test_init(&channel));

        CHECK(mliDynColor_malloc(&channel, 0));
        CHECK(mliDynColor_test_malloc(&channel, 0));

        for (i = 0; i < 1337 * 1337; i++) {
                struct mliColor c = mliColor_set(
                        1. * (float)i, 2. * (float)i, -1. * (float)i);
                CHECK(mliDynColor_push_back(&channel, c));
        }
        CHECK(channel.size == 1337 * 1337);
        CHECK(channel.array != NULL);
        CHECK(channel.capacity >= channel.size);

        for (i = 0; i < channel.size; i++) {
                CHECK_MARGIN(channel.array[i].r, 1. * (float)i, 1e-1);
                CHECK_MARGIN(channel.array[i].g, 2. * (float)i, 1e-1);
                CHECK_MARGIN(channel.array[i].b, -1. * (float)i, 1e-1);
        }
        mliDynColor_free(&channel);
        CHECK(mliDynColor_test_free(&channel));

        CHECK(mliDynColor_malloc(&channel, 100));
        CHECK(mliDynColor_test_malloc(&channel, 100));
        mliDynColor_free(&channel);
        CHECK(mliDynColor_test_free(&channel));
}

/*
CASE("num_pulses ExtractChannels")
{
        uint64_t num_channel_scenarios = 7;
        uint64_t channel_scenarios[7] = {0, 1, 10, 100, 3, 4, 5};
        uint64_t chsc = 0;

        uint64_t num_pulse_scenarios = 9;
        uint64_t pulse_scenarios[9] = {0, 1, 10, 50, 0, 3, 64, 12, 1337};
        uint64_t pusc = 0;

        for (chsc = 0; chsc < num_channel_scenarios; chsc++) {
                for (pusc = 0; pusc < num_pulse_scenarios; pusc++) {
                        uint64_t p = 0;
                        uint64_t c = 0;
                        uint64_t expected_num_pulses =
                                channel_scenarios[chsc] * pulse_scenarios[pusc];

                        struct mliColor ex;
                        struct mliColorChannels phs = mliColorChannels_init();

                        CHECK(mliColorChannels_malloc(
                                &phs, channel_scenarios[chsc]));
                        CHECK(phs.num_channels == channel_scenarios[chsc]);

                        for (c = 0; c < phs.num_channels; c++) {
                                for (p = 0; p < pulse_scenarios[pusc]; p++) {
                                        ex.r = p + 1 + c;
                                        ex.g = p + c;
                                        ex.b = 0;
                                        CHECK(mliDynColor_push_back(
                                                &phs.channels[c], ex));
                                }
                        }
                        CHECK(mliColorChannels_total_num(&phs) ==
                              expected_num_pulses);
                        mliColorChannels_free(&phs);
                }
        }
}
*/
