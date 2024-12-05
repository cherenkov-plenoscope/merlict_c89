/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mli_Pixels")
{
        uint64_t i;
        struct mli_Pixels p = mli_Pixels_init();
        CHECK(p.capacity == 0u);
        CHECK(p.array == NULL);

        CHECK(mli_Pixels_malloc(&p, 5u));
        CHECK(p.capacity == 5u);
        for (i = 0; i < 5; i++) {
                p.array[i].row = i;
                p.array[i].col = i + 1;
        }
        for (i = 0; i < 5; i++) {
                CHECK(p.array[i].row == i);
                CHECK(p.array[i].col == i + 1);
        }
        mli_Pixels_free(&p);

        CHECK(p.capacity == 0u);
        CHECK(p.array == NULL);

        CHECK(mli_Pixels_malloc(&p, 10));
        mli_Pixels_free(&p);

        CHECK(p.capacity == 0u);
        CHECK(p.array == NULL);
}
