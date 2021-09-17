/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mliImage_malloc")
{
        struct mliImage img = mliImage_init();
        CHECK(mliImage_malloc(&img, 3u, 2u));
        CHECK(img.num_cols == 3u);
        CHECK(img.num_rows == 2u);
        mliImage_free(&img);
        CHECK(img.num_cols == 0u);
        CHECK(img.num_rows == 0u);
}

CASE("scaling")
{
        struct mliImage src = mliImage_init();
        struct mliImage dst = mliImage_init();
        CHECK(mliImage_malloc(&src, 4, 4));
        CHECK(mliImage_malloc(&dst, 2, 2));
        /*
         *    src:
         *             0    1    2    3  cols
         *         _____________________
         *    0   |   10   20   30   40
         *    1   |   50   60   70   80
         *    2   |   55   34   31   17
         *    3   |   98   26   11   23
         *   rows
         */
        mliImage_set(&src, 0, 0, mliColor_set(0, 0, 10));
        mliImage_set(&src, 0, 1, mliColor_set(0, 0, 20));
        mliImage_set(&src, 0, 2, mliColor_set(0, 0, 30));
        mliImage_set(&src, 0, 3, mliColor_set(0, 0, 40));
        mliImage_set(&src, 1, 0, mliColor_set(0, 0, 50));
        mliImage_set(&src, 1, 1, mliColor_set(0, 0, 60));
        mliImage_set(&src, 1, 2, mliColor_set(0, 0, 70));
        mliImage_set(&src, 1, 3, mliColor_set(0, 0, 80));
        mliImage_set(&src, 2, 0, mliColor_set(0, 0, 55));
        mliImage_set(&src, 2, 1, mliColor_set(0, 0, 34));
        mliImage_set(&src, 2, 2, mliColor_set(0, 0, 31));
        mliImage_set(&src, 2, 3, mliColor_set(0, 0, 17));
        mliImage_set(&src, 3, 0, mliColor_set(0, 0, 98));
        mliImage_set(&src, 3, 1, mliColor_set(0, 0, 26));
        mliImage_set(&src, 3, 2, mliColor_set(0, 0, 11));
        mliImage_set(&src, 3, 3, mliColor_set(0, 0, 23));

        mliImage_scale_down_twice(&src, &dst);

        CHECK_MARGIN(
                mliImage_at(&dst, 0, 0).b, (10. + 20. + 50. + 60.) / 4., 1e-6);
        CHECK_MARGIN(
                mliImage_at(&dst, 0, 1).b, (30. + 40. + 70. + 80.) / 4., 1e-6);
        CHECK_MARGIN(
                mliImage_at(&dst, 1, 0).b, (55. + 34. + 98. + 26.) / 4., 1e-6);
        CHECK_MARGIN(
                mliImage_at(&dst, 1, 1).b, (31. + 17. + 11. + 23.) / 4., 1e-6);
        mliImage_free(&src);
        mliImage_free(&dst);
}

CASE("mliImage_write_to_ppm, mliImage_malloc_from_ppm")
{
        const char path[] = "merlict_c89/tests/resources/img.ppm.tmp";
        struct mliImage img = mliImage_init();
        struct mliImage back = mliImage_init();
        uint32_t col;
        uint32_t row;
        double tone;
        CHECK(mliImage_malloc(&img, 3, 2));
        tone = 0.;
        for (col = 0; col < img.num_cols; col++) {
                for (row = 0; row < img.num_rows; row++) {
                        struct mliColor color;
                        tone = (double)col * (double)row;
                        color.r = tone;
                        color.g = tone + 1.;
                        color.b = tone + 2.;
                        mliImage_set(&img, col, row, color);
                }
        }
        mliImage_write_to_ppm(&img, path);

        CHECK(mliImage_malloc_from_ppm(&back, path));
        CHECK(back.num_cols == 3u);
        CHECK(back.num_rows == 2u);

        for (col = 0; col < back.num_cols; col++) {
                for (row = 0; row < back.num_rows; row++) {
                        struct mliColor c_in = mliImage_at(&img, col, row);
                        struct mliColor c_back = mliImage_at(&back, col, row);
                        CHECK_MARGIN(c_in.r, c_back.r, 1.);
                        CHECK_MARGIN(c_in.g, c_back.g, 1.);
                        CHECK_MARGIN(c_in.b, c_back.b, 1.);
                }
        }

        mliImage_free(&img);
        mliImage_free(&back);
}


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
