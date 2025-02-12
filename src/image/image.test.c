/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mli_Image_init")
{
        struct mli_Image img = mli_Image_init();
        CHECK(mli_Image_num_cols(&img) == 0u);
        CHECK(mli_Image_num_rows(&img) == 0u);
        CHECK(img.chunks == NULL);
}

CASE("mli_Image_free")
{
        struct mli_Image img = mli_Image_init();
        mli_Image_free(&img);
}

CASE("mli_Image_malloc")
{
        struct mli_Image img = mli_Image_init();
        CHECK(mli_Image_malloc(&img, 3u, 2u));
        CHECK(mli_Image_num_cols(&img) == 3u);
        CHECK(mli_Image_num_rows(&img) == 2u);
        mli_Image_free(&img);
        CHECK(mli_Image_num_cols(&img) == 0u);
        CHECK(mli_Image_num_rows(&img) == 0u);
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
        mli_Image_set_by_col_row(&src, 0, 0, mli_Color_set(0, 0, 10));
        mli_Image_set_by_col_row(&src, 0, 1, mli_Color_set(0, 0, 20));
        mli_Image_set_by_col_row(&src, 0, 2, mli_Color_set(0, 0, 30));
        mli_Image_set_by_col_row(&src, 0, 3, mli_Color_set(0, 0, 40));
        mli_Image_set_by_col_row(&src, 1, 0, mli_Color_set(0, 0, 50));
        mli_Image_set_by_col_row(&src, 1, 1, mli_Color_set(0, 0, 60));
        mli_Image_set_by_col_row(&src, 1, 2, mli_Color_set(0, 0, 70));
        mli_Image_set_by_col_row(&src, 1, 3, mli_Color_set(0, 0, 80));
        mli_Image_set_by_col_row(&src, 2, 0, mli_Color_set(0, 0, 55));
        mli_Image_set_by_col_row(&src, 2, 1, mli_Color_set(0, 0, 34));
        mli_Image_set_by_col_row(&src, 2, 2, mli_Color_set(0, 0, 31));
        mli_Image_set_by_col_row(&src, 2, 3, mli_Color_set(0, 0, 17));
        mli_Image_set_by_col_row(&src, 3, 0, mli_Color_set(0, 0, 98));
        mli_Image_set_by_col_row(&src, 3, 1, mli_Color_set(0, 0, 26));
        mli_Image_set_by_col_row(&src, 3, 2, mli_Color_set(0, 0, 11));
        mli_Image_set_by_col_row(&src, 3, 3, mli_Color_set(0, 0, 23));

        mli_Image_scale_down_twice(&src, &dst);

        CHECK_MARGIN(
                mli_Image_get_by_col_row(&dst, 0, 0).b,
                (10. + 20. + 50. + 60.) / 4.,
                1e-6);
        CHECK_MARGIN(
                mli_Image_get_by_col_row(&dst, 0, 1).b,
                (30. + 40. + 70. + 80.) / 4.,
                1e-6);
        CHECK_MARGIN(
                mli_Image_get_by_col_row(&dst, 1, 0).b,
                (55. + 34. + 98. + 26.) / 4.,
                1e-6);
        CHECK_MARGIN(
                mli_Image_get_by_col_row(&dst, 1, 1).b,
                (31. + 17. + 11. + 23.) / 4.,
                1e-6);
        mli_Image_free(&src);
        mli_Image_free(&dst);
}

CASE("mli_Image_sobel")
{
        int32_t col;
        int32_t row;

        const int32_t BOX_X_START = 30;
        const int32_t BOX_X_STOP = 60;
        const int32_t BOX_Y_START = 20;
        const int32_t BOX_Y_STOP = 50;

        struct mli_Image src = mli_Image_init();
        struct mli_Image dst = mli_Image_init();
        CHECK(mli_Image_malloc(&src, 160, 90));

        mli_Image_set_all(&src, mli_Color_set(0.5, 0.5, 0.5));

        for (col = BOX_X_START; col < BOX_X_STOP; col++) {
                for (row = BOX_Y_START; row < BOX_Y_STOP; row++) {
                        mli_Image_set_by_col_row(
                                &src, col, row, mli_Color_set(1.0, 0, 0));
                }
        }

        CHECK(mli_Image_sobel(&src, &dst));

        for (col = BOX_X_START + 1; col < BOX_X_STOP - 1; col++) {
                for (row = BOX_Y_START + 1; row < BOX_Y_STOP - 1; row++) {
                        struct mli_Color c =
                                mli_Image_get_by_col_row(&dst, col, row);
                        float l = mli_Color_luminance(c);
                        CHECK_MARGIN(l, 0.0, 1e-2);
                }
        }

        mli_Image_free(&src);
        mli_Image_free(&dst);
}
