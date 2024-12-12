/* Copyright 2020-2021 Sebastian Achim Mueller                                */

CASE("mli_image_PixelWalk, no rest")
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
        struct mli_image_PixelWalk w =
                mli_image_PixelWalk_set(num_cols, num_rows, 2);
        CHECK(w.chunk_edge_size == 2);
        CHECK(w.num_cols == num_cols);
        CHECK(w.num_rows == num_rows);

        /* chunk 0,0 */
        CHECK(w.chunk_col == 0 && w.chunk_row == 0);
        CHECK(mli_image_PixelWalk_get(&w).col == 0 &&
              mli_image_PixelWalk_get(&w).row == 0);
        mli_image_PixelWalk_walk(&w);
        CHECK(mli_image_PixelWalk_get(&w).col == 0 &&
              mli_image_PixelWalk_get(&w).row == 1);
        mli_image_PixelWalk_walk(&w);
        CHECK(mli_image_PixelWalk_get(&w).col == 1 &&
              mli_image_PixelWalk_get(&w).row == 0);
        mli_image_PixelWalk_walk(&w);
        CHECK(mli_image_PixelWalk_get(&w).col == 1 &&
              mli_image_PixelWalk_get(&w).row == 1);

        /* chunk 0,1 */
        mli_image_PixelWalk_walk(&w);
        CHECK(w.chunk_col == 0 && w.chunk_row == 1);
        CHECK(mli_image_PixelWalk_get(&w).col == 0 &&
              mli_image_PixelWalk_get(&w).row == 2);
        mli_image_PixelWalk_walk(&w);
        CHECK(mli_image_PixelWalk_get(&w).col == 0 &&
              mli_image_PixelWalk_get(&w).row == 3);
        mli_image_PixelWalk_walk(&w);
        CHECK(mli_image_PixelWalk_get(&w).col == 1 &&
              mli_image_PixelWalk_get(&w).row == 2);
        mli_image_PixelWalk_walk(&w);
        CHECK(mli_image_PixelWalk_get(&w).col == 1 &&
              mli_image_PixelWalk_get(&w).row == 3);

        /* chunk 1,0 */
        mli_image_PixelWalk_walk(&w);
        CHECK(w.chunk_col == 1 && w.chunk_row == 0);
        CHECK(mli_image_PixelWalk_get(&w).col == 2 &&
              mli_image_PixelWalk_get(&w).row == 0);
        mli_image_PixelWalk_walk(&w);
        CHECK(mli_image_PixelWalk_get(&w).col == 2 &&
              mli_image_PixelWalk_get(&w).row == 1);
        mli_image_PixelWalk_walk(&w);
        CHECK(mli_image_PixelWalk_get(&w).col == 3 &&
              mli_image_PixelWalk_get(&w).row == 0);
        mli_image_PixelWalk_walk(&w);
        CHECK(mli_image_PixelWalk_get(&w).col == 3 &&
              mli_image_PixelWalk_get(&w).row == 1);

        /* chunk 1,1 */
        mli_image_PixelWalk_walk(&w);
        CHECK(w.chunk_col == 1 && w.chunk_row == 1);
        CHECK(mli_image_PixelWalk_get(&w).col == 2 &&
              mli_image_PixelWalk_get(&w).row == 2);
        mli_image_PixelWalk_walk(&w);
        CHECK(mli_image_PixelWalk_get(&w).col == 2 &&
              mli_image_PixelWalk_get(&w).row == 3);
        mli_image_PixelWalk_walk(&w);
        CHECK(mli_image_PixelWalk_get(&w).col == 3 &&
              mli_image_PixelWalk_get(&w).row == 2);
        mli_image_PixelWalk_walk(&w);
        CHECK(mli_image_PixelWalk_get(&w).col == 3 &&
              mli_image_PixelWalk_get(&w).row == 3);

        /* chunk 2,0 */
        mli_image_PixelWalk_walk(&w);
        CHECK(w.chunk_col == 2 && w.chunk_row == 0);
        CHECK(mli_image_PixelWalk_get(&w).col == 4 &&
              mli_image_PixelWalk_get(&w).row == 0);
        mli_image_PixelWalk_walk(&w);
        CHECK(mli_image_PixelWalk_get(&w).col == 4 &&
              mli_image_PixelWalk_get(&w).row == 1);
        mli_image_PixelWalk_walk(&w);
        CHECK(mli_image_PixelWalk_get(&w).col == 5 &&
              mli_image_PixelWalk_get(&w).row == 0);
        mli_image_PixelWalk_walk(&w);
        CHECK(mli_image_PixelWalk_get(&w).col == 5 &&
              mli_image_PixelWalk_get(&w).row == 1);

        /* chunk 2,1 */
        mli_image_PixelWalk_walk(&w);
        CHECK(w.chunk_col == 2 && w.chunk_row == 1);
        CHECK(mli_image_PixelWalk_get(&w).col == 4 &&
              mli_image_PixelWalk_get(&w).row == 2);
        mli_image_PixelWalk_walk(&w);
        CHECK(mli_image_PixelWalk_get(&w).col == 4 &&
              mli_image_PixelWalk_get(&w).row == 3);
        mli_image_PixelWalk_walk(&w);
        CHECK(mli_image_PixelWalk_get(&w).col == 5 &&
              mli_image_PixelWalk_get(&w).row == 2);
        mli_image_PixelWalk_walk(&w);
        CHECK(mli_image_PixelWalk_get(&w).col == 5 &&
              mli_image_PixelWalk_get(&w).row == 3);

        /* back to chunk 0,0 */
        mli_image_PixelWalk_walk(&w);
        CHECK(w.chunk_col == 0 && w.chunk_row == 0);
        CHECK(mli_image_PixelWalk_get(&w).col == 0 &&
              mli_image_PixelWalk_get(&w).row == 0);
}

CASE("mli_image_PixelWalk, with rest")
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
        struct mli_image_PixelWalk w =
                mli_image_PixelWalk_set(num_cols, num_rows, 2);
        CHECK(w.chunk_edge_size == 2);
        CHECK(w.num_cols == num_cols);
        CHECK(w.num_rows == num_rows);

        /* chunk 0,0 */
        CHECK(w.chunk_col == 0 && w.chunk_row == 0);
        CHECK(mli_image_PixelWalk_get(&w).col == 0 &&
              mli_image_PixelWalk_get(&w).row == 0);
        mli_image_PixelWalk_walk(&w);
        CHECK(w.chunk_col == 0 && w.chunk_row == 0);
        CHECK(mli_image_PixelWalk_get(&w).col == 0 &&
              mli_image_PixelWalk_get(&w).row == 1);
        mli_image_PixelWalk_walk(&w);
        CHECK(w.chunk_col == 0 && w.chunk_row == 0);
        CHECK(mli_image_PixelWalk_get(&w).col == 1 &&
              mli_image_PixelWalk_get(&w).row == 0);
        mli_image_PixelWalk_walk(&w);
        CHECK(w.chunk_col == 0 && w.chunk_row == 0);
        CHECK(mli_image_PixelWalk_get(&w).col == 1 &&
              mli_image_PixelWalk_get(&w).row == 1);

        /* chunk 0,1 */
        mli_image_PixelWalk_walk(&w);
        CHECK(w.chunk_col == 0 && w.chunk_row == 1);
        CHECK(mli_image_PixelWalk_get(&w).col == 0 &&
              mli_image_PixelWalk_get(&w).row == 2);
        mli_image_PixelWalk_walk(&w);
        CHECK(w.chunk_col == 0 && w.chunk_row == 1);
        CHECK(mli_image_PixelWalk_get(&w).col == 1 &&
              mli_image_PixelWalk_get(&w).row == 2);

        /* chunk 1,0 */
        mli_image_PixelWalk_walk(&w);
        CHECK(w.chunk_col == 1 && w.chunk_row == 0);
        CHECK(mli_image_PixelWalk_get(&w).col == 2 &&
              mli_image_PixelWalk_get(&w).row == 0);
        mli_image_PixelWalk_walk(&w);
        CHECK(w.chunk_col == 1 && w.chunk_row == 0);
        CHECK(mli_image_PixelWalk_get(&w).col == 2 &&
              mli_image_PixelWalk_get(&w).row == 1);
        mli_image_PixelWalk_walk(&w);
        CHECK(w.chunk_col == 1 && w.chunk_row == 0);
        CHECK(mli_image_PixelWalk_get(&w).col == 3 &&
              mli_image_PixelWalk_get(&w).row == 0);
        mli_image_PixelWalk_walk(&w);
        CHECK(w.chunk_col == 1 && w.chunk_row == 0);
        CHECK(mli_image_PixelWalk_get(&w).col == 3 &&
              mli_image_PixelWalk_get(&w).row == 1);

        /* chunk 1,1 */
        mli_image_PixelWalk_walk(&w);
        CHECK(w.chunk_col == 1 && w.chunk_row == 1);
        CHECK(mli_image_PixelWalk_get(&w).col == 2 &&
              mli_image_PixelWalk_get(&w).row == 2);
        mli_image_PixelWalk_walk(&w);
        CHECK(w.chunk_col == 1 && w.chunk_row == 1);
        CHECK(mli_image_PixelWalk_get(&w).col == 3 &&
              mli_image_PixelWalk_get(&w).row == 2);

        /* chunk 2,0 */
        mli_image_PixelWalk_walk(&w);
        CHECK(w.chunk_col == 2 && w.chunk_row == 0);
        CHECK(mli_image_PixelWalk_get(&w).col == 4 &&
              mli_image_PixelWalk_get(&w).row == 0);
        mli_image_PixelWalk_walk(&w);
        CHECK(w.chunk_col == 2 && w.chunk_row == 0);
        CHECK(mli_image_PixelWalk_get(&w).col == 4 &&
              mli_image_PixelWalk_get(&w).row == 1);

        /* chunk 2,1 */
        mli_image_PixelWalk_walk(&w);
        CHECK(w.chunk_col == 2 && w.chunk_row == 1);
        CHECK(mli_image_PixelWalk_get(&w).col == 4 &&
              mli_image_PixelWalk_get(&w).row == 2);

        /* back to chunk 0,0 */
        mli_image_PixelWalk_walk(&w);
        CHECK(w.chunk_col == 0 && w.chunk_row == 0);
        CHECK(mli_image_PixelWalk_get(&w).col == 0 &&
              mli_image_PixelWalk_get(&w).row == 0);
}
