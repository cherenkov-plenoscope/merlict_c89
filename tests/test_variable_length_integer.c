/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("dynUint8")
{
        uint64_t i;
        struct mliMT19937 prng = mliMT19937_init(0);
        struct mliDynUint8 a = mliDynUint8_init();
        struct mliDynUint8 b = mliDynUint8_init();
        struct mliDynUint8 r = mliDynUint8_init();
        CHECK(mliDynUint8_malloc(&a, 0));
        CHECK(mliDynUint8_malloc(&b, 0));
        CHECK(mliDynUint8_malloc(&r, 0));

        for (i = 0; i < 100; i++) {
                uint64_t aa, bb, rr;
                aa = (uint64_t)(
                        mli_random_uniform(&prng) * (1000 * 1000 * 1000));
                bb = (uint64_t)(
                        mli_random_uniform(&prng) * (1000 * 1000 * 1000));

                CHECK(mli_decimal_to_base255(aa, &a));
                CHECK(mli_base255_to_decimal(&a) == aa);

                CHECK(mli_decimal_to_base255(bb, &b));
                CHECK(mli_base255_to_decimal(&b) == bb);

                if (aa > bb) {
                        CHECK(mliDynUint8_greater_than(&a, &b));
                } else {
                        CHECK(!mliDynUint8_greater_than(&a, &b));
                }

                if (aa == bb) {
                        CHECK(mliDynUint8_equal(&a, &b));
                } else {
                        CHECK(!mliDynUint8_equal(&a, &b));
                }

                CHECK(mliDynUint8_add(&a, &b, &r));
                CHECK(r.dyn.size >= MLI_MAX2(a.dyn.size, b.dyn.size));
                rr = mli_base255_to_decimal(&r);
                CHECK(aa + bb == rr);
                if (rr > bb) {
                        CHECK(mliDynUint8_greater_than(&r, &b));
                }
                if (rr > aa) {
                        CHECK(mliDynUint8_greater_than(&r, &a));
                }

                CHECK(mliDynUint8_divide_two(&a, &r));
                rr = mli_base255_to_decimal(&r);
                CHECK(aa / 2 == rr);
        }

        mliDynUint8_free(&a);
        mliDynUint8_free(&b);
        mliDynUint8_free(&r);
}

CASE("dynUint8_from_string")
{
        uint64_t i;
        struct mliDynUint8 a = mliDynUint8_init();
        char text[6][100] = {
                "peter",
                "klaus",
                "nebert\n",
                "FoU\t123",
                "0123",
                " whitespace ",
        };
        CHECK(mliDynUint8_malloc(&a, 0));
        for (i = 0; i < 6; i++) {
                CHECK(mliDynUint8_malloc_char(&a, text[i]));
                CHECK(a.dyn.size == strlen(text[i]) + 1);
                CHECK(a.arr[strlen(text[i])] == '\0');
        }
        mliDynUint8_free(&a);
}
