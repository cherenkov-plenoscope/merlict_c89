/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "mli_testing.h"
#include "mliPixels.h"

CASE("mliPixels")
{
        uint64_t i;
        struct mliPixels p = mliPixels_init();
        CHECK(p.num_pixels == 0u);
        CHECK(p.pixels == NULL);

        CHECK(mliPixels_malloc(&p, 5u));
        CHECK(p.num_pixels == 5u);
        for (i = 0; i < 5; i++) {
                p.pixels[i].row = i;
                p.pixels[i].col = i + 1;
        }
        for (i = 0; i < 5; i++) {
                CHECK(p.pixels[i].row == i);
                CHECK(p.pixels[i].col == i + 1);
        }
        mliPixels_free(&p);

        CHECK(p.num_pixels == 0u);
        CHECK(p.pixels == NULL);

        CHECK(mliPixels_malloc(&p, 10));
        mliPixels_free(&p);

        CHECK(p.num_pixels == 0u);
        CHECK(p.pixels == NULL);
}
