/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mli_image_PixelVector")
{
        uint64_t i;
        struct mli_image_PixelVector p = mli_image_PixelVector_init();
        CHECK(p.capacity == 0u);
        CHECK(p.array == NULL);

        CHECK(mli_image_PixelVector_malloc(&p, 5u));
        CHECK(p.capacity == 5u);
        for (i = 0; i < 5; i++) {
                p.array[i].row = i;
                p.array[i].col = i + 1;
        }
        for (i = 0; i < 5; i++) {
                CHECK(p.array[i].row == i);
                CHECK(p.array[i].col == i + 1);
        }
        mli_image_PixelVector_free(&p);

        CHECK(p.capacity == 0u);
        CHECK(p.array == NULL);

        CHECK(mli_image_PixelVector_malloc(&p, 10));
        mli_image_PixelVector_free(&p);

        CHECK(p.capacity == 0u);
        CHECK(p.array == NULL);
}
