/* Copyright 2021 Sebastian A. Mueller */
#include "../../mli_testing/src/mli_testing.h"

CASE("mli_corsika_Histogram2d: init,malloc,free")
{
        struct mliCorsikaHistogram2d hist = mliCorsikaHistogram2d_init();
        CHECK(hist.dict.capacity == 0);

        CHECK(mliCorsikaHistogram2d_malloc(&hist, 100));
        CHECK(hist.dict.capacity == 100);

        mliCorsikaHistogram2d_free(&hist);
        CHECK(hist.dict.capacity == 0);
}

CASE("mli_corsika_Histogram2d: assign to same bin multiple times.")
{
        uint64_t i;
        struct mliCorsikaHistogram2d hist = mliCorsikaHistogram2d_init();
        struct mliDynCorsikaHistogram2dBin bins = mliDynCorsikaHistogram2dBin_init();

        CHECK(mliCorsikaHistogram2d_malloc(&hist, 10));
        CHECK(mliDynCorsikaHistogram2dBin_malloc(&bins, 1));

        for (i = 0; i < 20; i++) {
                CHECK(mliCorsikaHistogram2d_assign(&hist, 42, 13, 0.5));
        }

        CHECK(mliCorsikaHistogram2d_flatten(&hist, &bins));
        mliCorsikaHistogram2d_free(&hist);

        CHECK(bins.size == 1);
        CHECK(bins.array[0].x == 42);
        CHECK(bins.array[0].y == 13);
        CHECK_MARGIN(bins.array[0].value, 20 * 0.5, 1e-6);

        mliDynCorsikaHistogram2dBin_free(&bins);
}
