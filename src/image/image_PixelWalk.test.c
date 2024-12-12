/* Copyright 2020-2021 Sebastian Achim Mueller                                */

CASE("mli_image_PixelWalk, init zeros")
{
        struct mli_image_PixelWalk walk = mli_image_PixelWalk_init();
        CHECK(walk.chunk_row == 0);
        CHECK(walk.sub_row == 0);
        CHECK(walk.chunk_col == 0);
        CHECK(walk.sub_col == 0);
        CHECK(walk.i == 0);
}

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
        const uint32_t NUM_ROWS = 4;
        const uint32_t NUM_COLS = 6;
        const uint32_t CHUNK_SIZE = 2;
        struct mli_image_ChunkGeometry g =
                mli_image_ChunkGeometry_set(NUM_COLS, NUM_ROWS, CHUNK_SIZE);
        struct mli_image_PixelWalk w = mli_image_PixelWalk_init();
        CHECK(g.chunk_edge_size == CHUNK_SIZE);
        CHECK(g.num_cols == NUM_COLS);
        CHECK(g.num_rows == NUM_ROWS);

        /* chunk 0,0 */
        CHECK(w.chunk_col == 0 && w.chunk_row == 0);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 0 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 0);
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 0 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 1);
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 1 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 0);
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 1 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 1);

        /* chunk 0,1 */
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(w.chunk_col == 0 && w.chunk_row == 1);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 0 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 2);
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 0 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 3);
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 1 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 2);
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 1 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 3);

        /* chunk 1,0 */
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(w.chunk_col == 1 && w.chunk_row == 0);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 2 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 0);
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 2 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 1);
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 3 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 0);
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 3 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 1);

        /* chunk 1,1 */
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(w.chunk_col == 1 && w.chunk_row == 1);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 2 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 2);
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 2 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 3);
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 3 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 2);
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 3 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 3);

        /* chunk 2,0 */
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(w.chunk_col == 2 && w.chunk_row == 0);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 4 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 0);
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 4 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 1);
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 5 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 0);
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 5 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 1);

        /* chunk 2,1 */
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(w.chunk_col == 2 && w.chunk_row == 1);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 4 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 2);
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 4 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 3);
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 5 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 2);
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 5 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 3);

        /* back to chunk 0,0 */
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(w.chunk_col == 0 && w.chunk_row == 0);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 0 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 0);
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
        const uint32_t NUM_ROWS = 3;
        const uint32_t NUM_COLS = 5;
        const uint32_t CHUNK_SIZE = 2;
        struct mli_image_ChunkGeometry g =
                mli_image_ChunkGeometry_set(NUM_COLS, NUM_ROWS, CHUNK_SIZE);
        struct mli_image_PixelWalk w = mli_image_PixelWalk_init();
        CHECK(g.chunk_edge_size == CHUNK_SIZE);
        CHECK(g.num_cols == NUM_COLS);
        CHECK(g.num_rows == NUM_ROWS);

        /* chunk 0,0 */
        CHECK(w.chunk_col == 0 && w.chunk_row == 0);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 0 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 0);
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(w.chunk_col == 0 && w.chunk_row == 0);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 0 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 1);
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(w.chunk_col == 0 && w.chunk_row == 0);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 1 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 0);
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(w.chunk_col == 0 && w.chunk_row == 0);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 1 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 1);

        /* chunk 0,1 */
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(w.chunk_col == 0 && w.chunk_row == 1);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 0 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 2);
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(w.chunk_col == 0 && w.chunk_row == 1);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 1 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 2);

        /* chunk 1,0 */
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(w.chunk_col == 1 && w.chunk_row == 0);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 2 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 0);
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(w.chunk_col == 1 && w.chunk_row == 0);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 2 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 1);
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(w.chunk_col == 1 && w.chunk_row == 0);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 3 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 0);
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(w.chunk_col == 1 && w.chunk_row == 0);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 3 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 1);

        /* chunk 1,1 */
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(w.chunk_col == 1 && w.chunk_row == 1);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 2 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 2);
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(w.chunk_col == 1 && w.chunk_row == 1);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 3 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 2);

        /* chunk 2,0 */
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(w.chunk_col == 2 && w.chunk_row == 0);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 4 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 0);
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(w.chunk_col == 2 && w.chunk_row == 0);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 4 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 1);

        /* chunk 2,1 */
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(w.chunk_col == 2 && w.chunk_row == 1);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 4 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 2);

        /* back to chunk 0,0 */
        mli_image_PixelWalk_walk(&w, &g);
        CHECK(w.chunk_col == 0 && w.chunk_row == 0);
        CHECK(mli_image_PixelWalk_get_Pixel(&w, &g).col == 0 &&
              mli_image_PixelWalk_get_Pixel(&w, &g).row == 0);
}
