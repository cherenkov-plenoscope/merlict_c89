/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mliPixels") {
    uint64_t i;
    mliPixels p = mliPixels_init();
    CHECK(p.num_pixels == 0u);
    CHECK(p.pixels == NULL);

    p.num_pixels = 5;
    CHECK(mliPixels_malloc(&p));
    for (i = 0; i < 5; i ++) {
        p.pixels[i].row = i;
        p.pixels[i].col = i + 1;}
    for (i = 0; i < 5; i ++) {
        CHECK(p.pixels[i].row == i);
        CHECK(p.pixels[i].col == i + 1);}
    mliPixels_free(&p);

    CHECK(p.num_pixels == 0u);
    CHECK(p.pixels == NULL);

    p.num_pixels = 10;
    CHECK(mliPixels_malloc(&p));
    mliPixels_free(&p);

    CHECK(p.num_pixels == 0u);
    CHECK(p.pixels == NULL);
}
