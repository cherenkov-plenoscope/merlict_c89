/* Copyright 2020-2021 Sebastian Achim Mueller                                */

#include "../../mli_testing/src/mli_testing.h"
#include "../src/mliPixelWalk.h"

CASE("mliPixelWalk, no rest")
{
        /*
                 0  1  2  3  4  5 cols->

            0    0  2  8 10 16 18
            1    1  3  9 11 17 19
            2    4  6 12 14 20 22
            3    5  7 13 15 21 23

            r
            o
            w
            s
            |
            v
         */
        uint32_t num_rows = 4;
        uint32_t num_cols = 6;
        struct mliPixelWalk w = mliPixelWalk_set(num_cols, num_rows, 2);
        CHECK(w.chunk_size == 2);
        CHECK(w.num_cols == num_cols);
        CHECK(w.num_rows == num_rows);

        /* chunk 0,0 */
        CHECK(w.chunk_col == 0 && w.chunk_row == 0);
        CHECK(mliPixelWalk_get(&w).col == 0 && mliPixelWalk_get(&w).row == 0);
        mliPixelWalk_walk(&w);
        CHECK(mliPixelWalk_get(&w).col == 0 && mliPixelWalk_get(&w).row == 1);
        mliPixelWalk_walk(&w);
        CHECK(mliPixelWalk_get(&w).col == 1 && mliPixelWalk_get(&w).row == 0);
        mliPixelWalk_walk(&w);
        CHECK(mliPixelWalk_get(&w).col == 1 && mliPixelWalk_get(&w).row == 1);

        /* chunk 0,1 */
        mliPixelWalk_walk(&w);
        CHECK(w.chunk_col == 0 && w.chunk_row == 1);
        CHECK(mliPixelWalk_get(&w).col == 0 && mliPixelWalk_get(&w).row == 2);
        mliPixelWalk_walk(&w);
        CHECK(mliPixelWalk_get(&w).col == 0 && mliPixelWalk_get(&w).row == 3);
        mliPixelWalk_walk(&w);
        CHECK(mliPixelWalk_get(&w).col == 1 && mliPixelWalk_get(&w).row == 2);
        mliPixelWalk_walk(&w);
        CHECK(mliPixelWalk_get(&w).col == 1 && mliPixelWalk_get(&w).row == 3);

        /* chunk 1,0 */
        mliPixelWalk_walk(&w);
        CHECK(w.chunk_col == 1 && w.chunk_row == 0);
        CHECK(mliPixelWalk_get(&w).col == 2 && mliPixelWalk_get(&w).row == 0);
        mliPixelWalk_walk(&w);
        CHECK(mliPixelWalk_get(&w).col == 2 && mliPixelWalk_get(&w).row == 1);
        mliPixelWalk_walk(&w);
        CHECK(mliPixelWalk_get(&w).col == 3 && mliPixelWalk_get(&w).row == 0);
        mliPixelWalk_walk(&w);
        CHECK(mliPixelWalk_get(&w).col == 3 && mliPixelWalk_get(&w).row == 1);

        /* chunk 1,1 */
        mliPixelWalk_walk(&w);
        CHECK(w.chunk_col == 1 && w.chunk_row == 1);
        CHECK(mliPixelWalk_get(&w).col == 2 && mliPixelWalk_get(&w).row == 2);
        mliPixelWalk_walk(&w);
        CHECK(mliPixelWalk_get(&w).col == 2 && mliPixelWalk_get(&w).row == 3);
        mliPixelWalk_walk(&w);
        CHECK(mliPixelWalk_get(&w).col == 3 && mliPixelWalk_get(&w).row == 2);
        mliPixelWalk_walk(&w);
        CHECK(mliPixelWalk_get(&w).col == 3 && mliPixelWalk_get(&w).row == 3);

        /* chunk 2,0 */
        mliPixelWalk_walk(&w);
        CHECK(w.chunk_col == 2 && w.chunk_row == 0);
        CHECK(mliPixelWalk_get(&w).col == 4 && mliPixelWalk_get(&w).row == 0);
        mliPixelWalk_walk(&w);
        CHECK(mliPixelWalk_get(&w).col == 4 && mliPixelWalk_get(&w).row == 1);
        mliPixelWalk_walk(&w);
        CHECK(mliPixelWalk_get(&w).col == 5 && mliPixelWalk_get(&w).row == 0);
        mliPixelWalk_walk(&w);
        CHECK(mliPixelWalk_get(&w).col == 5 && mliPixelWalk_get(&w).row == 1);

        /* chunk 2,1 */
        mliPixelWalk_walk(&w);
        CHECK(w.chunk_col == 2 && w.chunk_row == 1);
        CHECK(mliPixelWalk_get(&w).col == 4 && mliPixelWalk_get(&w).row == 2);
        mliPixelWalk_walk(&w);
        CHECK(mliPixelWalk_get(&w).col == 4 && mliPixelWalk_get(&w).row == 3);
        mliPixelWalk_walk(&w);
        CHECK(mliPixelWalk_get(&w).col == 5 && mliPixelWalk_get(&w).row == 2);
        mliPixelWalk_walk(&w);
        CHECK(mliPixelWalk_get(&w).col == 5 && mliPixelWalk_get(&w).row == 3);

        /* back to chunk 0,0 */
        mliPixelWalk_walk(&w);
        CHECK(w.chunk_col == 0 && w.chunk_row == 0);
        CHECK(mliPixelWalk_get(&w).col == 0 && mliPixelWalk_get(&w).row == 0);
}

CASE("mliPixelWalk, with rest")
{
        /*
                 0  1  2  3  4  5 cols->

            0    0  2  6  8 12  .
            1    1  3  7  9 13  .
            2    4  5 10 11 14  .
            3    .  .  .  .  .  .

            r
            o
            w
            s
            |
            v
         */
        uint32_t num_rows = 3;
        uint32_t num_cols = 5;
        struct mliPixelWalk w = mliPixelWalk_set(num_cols, num_rows, 2);
        CHECK(w.chunk_size == 2);
        CHECK(w.num_cols == num_cols);
        CHECK(w.num_rows == num_rows);

        /* chunk 0,0 */
        CHECK(w.chunk_col == 0 && w.chunk_row == 0);
        CHECK(mliPixelWalk_get(&w).col == 0 && mliPixelWalk_get(&w).row == 0);
        mliPixelWalk_walk(&w);
        CHECK(w.chunk_col == 0 && w.chunk_row == 0);
        CHECK(mliPixelWalk_get(&w).col == 0 && mliPixelWalk_get(&w).row == 1);
        mliPixelWalk_walk(&w);
        CHECK(w.chunk_col == 0 && w.chunk_row == 0);
        CHECK(mliPixelWalk_get(&w).col == 1 && mliPixelWalk_get(&w).row == 0);
        mliPixelWalk_walk(&w);
        CHECK(w.chunk_col == 0 && w.chunk_row == 0);
        CHECK(mliPixelWalk_get(&w).col == 1 && mliPixelWalk_get(&w).row == 1);

        /* chunk 0,1 */
        mliPixelWalk_walk(&w);
        CHECK(w.chunk_col == 0 && w.chunk_row == 1);
        CHECK(mliPixelWalk_get(&w).col == 0 && mliPixelWalk_get(&w).row == 2);
        mliPixelWalk_walk(&w);
        CHECK(w.chunk_col == 0 && w.chunk_row == 1);
        CHECK(mliPixelWalk_get(&w).col == 1 && mliPixelWalk_get(&w).row == 2);

        /* chunk 1,0 */
        mliPixelWalk_walk(&w);
        CHECK(w.chunk_col == 1 && w.chunk_row == 0);
        CHECK(mliPixelWalk_get(&w).col == 2 && mliPixelWalk_get(&w).row == 0);
        mliPixelWalk_walk(&w);
        CHECK(w.chunk_col == 1 && w.chunk_row == 0);
        CHECK(mliPixelWalk_get(&w).col == 2 && mliPixelWalk_get(&w).row == 1);
        mliPixelWalk_walk(&w);
        CHECK(w.chunk_col == 1 && w.chunk_row == 0);
        CHECK(mliPixelWalk_get(&w).col == 3 && mliPixelWalk_get(&w).row == 0);
        mliPixelWalk_walk(&w);
        CHECK(w.chunk_col == 1 && w.chunk_row == 0);
        CHECK(mliPixelWalk_get(&w).col == 3 && mliPixelWalk_get(&w).row == 1);

        /* chunk 1,1 */
        mliPixelWalk_walk(&w);
        CHECK(w.chunk_col == 1 && w.chunk_row == 1);
        CHECK(mliPixelWalk_get(&w).col == 2 && mliPixelWalk_get(&w).row == 2);
        mliPixelWalk_walk(&w);
        CHECK(w.chunk_col == 1 && w.chunk_row == 1);
        CHECK(mliPixelWalk_get(&w).col == 3 && mliPixelWalk_get(&w).row == 2);

        /* chunk 2,0 */
        mliPixelWalk_walk(&w);
        CHECK(w.chunk_col == 2 && w.chunk_row == 0);
        CHECK(mliPixelWalk_get(&w).col == 4 && mliPixelWalk_get(&w).row == 0);
        mliPixelWalk_walk(&w);
        CHECK(w.chunk_col == 2 && w.chunk_row == 0);
        CHECK(mliPixelWalk_get(&w).col == 4 && mliPixelWalk_get(&w).row == 1);

        /* chunk 2,1 */
        mliPixelWalk_walk(&w);
        CHECK(w.chunk_col == 2 && w.chunk_row == 1);
        CHECK(mliPixelWalk_get(&w).col == 4 && mliPixelWalk_get(&w).row == 2);

        /* back to chunk 0,0 */
        mliPixelWalk_walk(&w);
        CHECK(w.chunk_col == 0 && w.chunk_row == 0);
        CHECK(mliPixelWalk_get(&w).col == 0 && mliPixelWalk_get(&w).row == 0);
}
