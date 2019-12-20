/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mliImage_malloc") {
    mliImage img = mliImage_init();
    img.num_cols = 3u;
    img.num_rows = 2u;
    CHECK(mliImage_malloc(&img));
    CHECK(img.num_cols == 3u);
    CHECK(img.num_rows == 2u);
    mliImage_free(&img);
    CHECK(img.num_cols == 0u);
    CHECK(img.num_rows == 0u);
}

CASE("scaling") {
    mliImage src = mliImage_init();
    mliImage dst = mliImage_init();
    src.num_cols = 4u; src.num_rows = 4u;
    dst.num_cols = 2u; dst.num_rows = 2u;
    CHECK(mliImage_malloc(&src));
    CHECK(mliImage_malloc(&dst));
    /*
        src:
                 0    1    2    3  cols
             _____________________
        0   |   10   20   30   40
        1   |   50   60   70   80
        2   |   55   34   31   17
        3   |   98   26   11   23
       rows
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

    CHECK_MARGIN(mliImage_at(&dst, 0, 0).b, (10.+20.+50.+60.)/4., 1e-6);
    CHECK_MARGIN(mliImage_at(&dst, 0, 1).b, (30.+40.+70.+80.)/4., 1e-6);
    CHECK_MARGIN(mliImage_at(&dst, 1, 0).b, (55.+34.+98.+26.)/4., 1e-6);
    CHECK_MARGIN(mliImage_at(&dst, 1, 1).b, (31.+17.+11.+23.)/4., 1e-6);
    mliImage_free(&src);
    mliImage_free(&dst);
}

CASE("mliImage_write_to_ppm, mliImage_malloc_from_ppm") {
    mliImage img = mliImage_init();
    mliImage back = mliImage_init();
    uint32_t col;
    uint32_t row;
    double tone;
    img.num_cols = 3u;
    img.num_rows = 2u;
    CHECK(mliImage_malloc(&img));
    tone = 0.;
    for (col = 0; col < img.num_cols; col++) {
        for (row = 0; row < img.num_rows; row++) {
            mliColor color;
            tone = (double)col * (double)row;
            color.r = tone;
            color.g = tone + 1.;
            color.b = tone + 2.;
            mliImage_set(&img, col, row, color);
        }
    }
    mliImage_write_to_ppm(&img, "img.ppm.tmp");

    CHECK(mliImage_malloc_from_ppm(&back, "img.ppm.tmp"));
    CHECK(back.num_cols == 3u);
    CHECK(back.num_rows == 2u);

    for (col = 0; col < back.num_cols; col++) {
        for (row = 0; row < back.num_rows; row++) {
            mliColor c_in = mliImage_at(&img, col, row);
            mliColor c_back = mliImage_at(&back, col, row);
            CHECK_MARGIN(c_in.r, c_back.r, 1.);
            CHECK_MARGIN(c_in.g, c_back.g, 1.);
            CHECK_MARGIN(c_in.b, c_back.b, 1.);
        }
    }

    mliImage_free(&img);
    mliImage_free(&back);
}
