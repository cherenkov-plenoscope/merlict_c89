
CASE("mli_Image_ppm write/read color")
{
        struct mli_IO f = mli_IO_init();
        struct mli_Color original;
        struct mli_Color back;

        CHECK(mli_IO_open_memory(&f));

        original = mli_Color_set(13.0, 25.0, 178.0);

        /* 8 bit values in 8bit range */
        CHECK(mli_image_ppm__write_color_8bit(original, &f));
        mli_IO_rewind(&f);
        CHECK(mli_image_ppm__read_color_8bit(&back, &f));
        CHECK(mli_Color_equal_margin(original, back, 1e-6));

        mli_IO_rewind(&f);

        /* 16 bit and values in 8bit range */
        CHECK(mli_image_ppm__write_color_16bit(original, &f));
        mli_IO_rewind(&f);
        CHECK(mli_image_ppm__read_color_16bit(&back, &f));
        CHECK(mli_Color_equal_margin(original, back, 1e-6));

        mli_IO_rewind(&f);

        /* 16 bit and values exceed 8bit range */
        original = mli_Color_set(1100, 4321.0, 54321.0);
        CHECK(mli_image_ppm__write_color_16bit(original, &f));
        mli_IO_rewind(&f);
        CHECK(mli_image_ppm__read_color_16bit(&back, &f));
        CHECK(mli_Color_equal_margin(original, back, 1e-6));

        CHECK(mli_IO_close(&f));
}

CASE("mli_Image_write_to_ppm")
{
        const char path[] = "data/image/img.ppm.tmp";
        struct mli_IO f = mli_IO_init();
        struct mli_Image img = mli_Image_init();
        struct mli_Image back = mli_Image_init();
        int COLOR_DEPTHS[2] = {
                MLI_IMAGE_PPM_COLOR_DEPTH_8BIT,
                MLI_IMAGE_PPM_COLOR_DEPTH_16BIT};
        int c;
        uint32_t col;
        uint32_t row;
        double tone;
        CHECK(mli_Image_malloc(&img, 3, 2));
        tone = 0.;
        for (col = 0; col < mli_Image_num_cols(&img); col++) {
                for (row = 0; row < mli_Image_num_rows(&img); row++) {
                        struct mli_Color color;
                        tone = 1e-2 * (double)col * (double)row;
                        color.r = tone;
                        color.g = tone + 1e-2;
                        color.b = tone + 2e-2;
                        mli_Image_set_by_col_row(&img, col, row, color);
                }
        }

        for (c = 0; c < 2; c++) {
                CHECK(mli_IO_open_file_cstr(&f, path, "w"));
                CHECK(mli_Image_to_io(&img, &f, COLOR_DEPTHS[c]));
                CHECK(mli_IO_close(&f));

                CHECK(mli_IO_open_file_cstr(&f, path, "r"));
                CHECK(mli_Image_from_io(&back, &f));
                CHECK(mli_IO_close(&f));

                CHECK(mli_Image_num_cols(&back) == 3u);
                CHECK(mli_Image_num_rows(&back) == 2u);

                for (col = 0; col < mli_Image_num_cols(&back); col++) {
                        for (row = 0; row < mli_Image_num_rows(&back); row++) {
                                struct mli_Color c_in =
                                        mli_Image_get_by_col_row(
                                                &img, col, row);
                                struct mli_Color c_back =
                                        mli_Image_get_by_col_row(
                                                &back, col, row);
                                CHECK_MARGIN(c_in.r, c_back.r, 1e-2);
                                CHECK_MARGIN(c_in.g, c_back.g, 1e-2);
                                CHECK_MARGIN(c_in.b, c_back.b, 1e-2);
                        }
                }
        }
        mli_Image_free(&img);
        mli_Image_free(&back);
}
