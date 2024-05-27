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
        uint64_t i = 0;
        uint64_t a = 0;
        int32_t b = 0;
        double c = 0;
        int64_t rc = 0;

        struct mliCorsikaHistogram2d hist = mliCorsikaHistogram2d_init();
        struct mliIo buff = mliIo_init();


        CHECK(mliCorsikaHistogram2d_malloc(&hist, 10));
        CHECK(mliIo_malloc(&buff));

        for (i = 0; i < 20; i ++) {
                CHECK(mliCorsikaHistogram2d_assign(&hist, 42, 13, 0.5));
        }

        CHECK(mliCorsikaHistogram2d_dumps(&hist, &buff));
        mliCorsikaHistogram2d_free(&hist);

        buff.pos = 0;

        rc = mliIo_read(&buff, (const void *)(&a), sizeof(uint64_t), 1);
        CHECK(rc == 1);
        CHECK(a == 1);

        rc = mliIo_read(&buff, (const void *)(&b), sizeof(int32_t), 1);
        CHECK(rc == 1);
        CHECK(b == 42);

        rc = mliIo_read(&buff, (const void *)(&b), sizeof(int32_t), 1);
        CHECK(rc == 1);
        CHECK(b == 13);

        rc = mliIo_read(&buff, (const void *)(&c), sizeof(double), 1);
        CHECK(rc == 1);
        CHECK_MARGIN(c, 20 * 0.5, 1e-6);

        mliIo_free(&buff);
}
