/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mli_Image_malloc")
{
        struct mli_Image img = mli_Image_init();
        CHECK(mli_Image_malloc(&img, 3u, 2u));
        CHECK(img.num_cols == 3u);
        CHECK(img.num_rows == 2u);
        mli_Image_free(&img);
        CHECK(img.num_cols == 0u);
        CHECK(img.num_rows == 0u);
}

CASE("scaling")
{
        struct mli_Image src = mli_Image_init();
        struct mli_Image dst = mli_Image_init();
        CHECK(mli_Image_malloc(&src, 4, 4));
        CHECK(mli_Image_malloc(&dst, 2, 2));
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
        mli_Image_set(&src, 0, 0, mliColor_set(0, 0, 10));
        mli_Image_set(&src, 0, 1, mliColor_set(0, 0, 20));
        mli_Image_set(&src, 0, 2, mliColor_set(0, 0, 30));
        mli_Image_set(&src, 0, 3, mliColor_set(0, 0, 40));
        mli_Image_set(&src, 1, 0, mliColor_set(0, 0, 50));
        mli_Image_set(&src, 1, 1, mliColor_set(0, 0, 60));
        mli_Image_set(&src, 1, 2, mliColor_set(0, 0, 70));
        mli_Image_set(&src, 1, 3, mliColor_set(0, 0, 80));
        mli_Image_set(&src, 2, 0, mliColor_set(0, 0, 55));
        mli_Image_set(&src, 2, 1, mliColor_set(0, 0, 34));
        mli_Image_set(&src, 2, 2, mliColor_set(0, 0, 31));
        mli_Image_set(&src, 2, 3, mliColor_set(0, 0, 17));
        mli_Image_set(&src, 3, 0, mliColor_set(0, 0, 98));
        mli_Image_set(&src, 3, 1, mliColor_set(0, 0, 26));
        mli_Image_set(&src, 3, 2, mliColor_set(0, 0, 11));
        mli_Image_set(&src, 3, 3, mliColor_set(0, 0, 23));

        mli_Image_scale_down_twice(&src, &dst);

        CHECK_MARGIN(
                mli_Image_at(&dst, 0, 0).b, (10. + 20. + 50. + 60.) / 4., 1e-6);
        CHECK_MARGIN(
                mli_Image_at(&dst, 0, 1).b, (30. + 40. + 70. + 80.) / 4., 1e-6);
        CHECK_MARGIN(
                mli_Image_at(&dst, 1, 0).b, (55. + 34. + 98. + 26.) / 4., 1e-6);
        CHECK_MARGIN(
                mli_Image_at(&dst, 1, 1).b, (31. + 17. + 11. + 23.) / 4., 1e-6);
        mli_Image_free(&src);
        mli_Image_free(&dst);
}

CASE("mli_Image_write_to_ppm, mli_Image_malloc_from_ppm")
{
        const char path[] = "data/mli/tests/resources/image/img.ppm.tmp";
        struct mli_Image img = mli_Image_init();
        struct mli_Image back = mli_Image_init();
        uint32_t col;
        uint32_t row;
        double tone;
        CHECK(mli_Image_malloc(&img, 3, 2));
        tone = 0.;
        for (col = 0; col < img.num_cols; col++) {
                for (row = 0; row < img.num_rows; row++) {
                        struct mliColor color;
                        tone = (double)col * (double)row;
                        color.r = tone;
                        color.g = tone + 1.;
                        color.b = tone + 2.;
                        mli_Image_set(&img, col, row, color);
                }
        }
        mli_Image_write_to_path(&img, path);

        CHECK(mli_Image_malloc_from_path(&back, path));
        CHECK(back.num_cols == 3u);
        CHECK(back.num_rows == 2u);

        for (col = 0; col < back.num_cols; col++) {
                for (row = 0; row < back.num_rows; row++) {
                        struct mliColor c_in = mli_Image_at(&img, col, row);
                        struct mliColor c_back = mli_Image_at(&back, col, row);
                        CHECK_MARGIN(c_in.r, c_back.r, 1.);
                        CHECK_MARGIN(c_in.g, c_back.g, 1.);
                        CHECK_MARGIN(c_in.b, c_back.b, 1.);
                }
        }

        mli_Image_free(&img);
        mli_Image_free(&back);
}
