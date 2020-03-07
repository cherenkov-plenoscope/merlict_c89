/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("dynUint8") {
        size_t i;
        struct mliMT19937 prng = mliMT19937_init(0);
        struct mliDynUint8 a = mliDynUint8_init();
        struct mliDynUint8 b = mliDynUint8_init();
        struct mliDynUint8 r = mliDynUint8_init();
        CHECK(mliDynUint8_malloc(&a, 0));
        CHECK(mliDynUint8_malloc(&b, 0));
        CHECK(mliDynUint8_malloc(&r, 0));

        for (i = 0; i<100; i++) {
                uint64_t aa, bb, rr;
                aa = (uint64_t)(mli_random_uniform(&prng)*(1000*1000*1000));
                bb = (uint64_t)(mli_random_uniform(&prng)*(1000*1000*1000));

                CHECK(mli_decimal_to_base255(aa, &a));
                CHECK(mli_base255_to_decimal(&a) == aa);

                CHECK(mli_decimal_to_base255(bb, &b));
                CHECK(mli_base255_to_decimal(&b) == bb);

                if (aa > bb) {
                        CHECK(mliDynUint8_greater_than(&a, &b));
                } else {
                        CHECK(!mliDynUint8_greater_than(&a, &b));
                }

                CHECK(mliDynUint8_add(&a, &b, &r));
                CHECK(r.dyn.size >= MLI_MAX2(a.dyn.size, b.dyn.size));
                rr = mli_base255_to_decimal(&r);
                CHECK(aa + bb == rr);

                CHECK(mliDynUint8_divide_two(&a, &r));
                rr = mli_base255_to_decimal(&r);
                CHECK(aa/2 == rr);
        }

        mliDynUint8_free(&a);
        mliDynUint8_free(&b);
        mliDynUint8_free(&r);
}

CASE("dynUint8_from_string") {
        size_t i;
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


CASE("map") {
        uint64_t ret = 0;
        struct mliMap map = mliMap_init();
        CHECK(mliMap_has(&map, "hans") == 0);
        CHECK(mliMap_malloc_insert(&map, "hans", 1337));
        CHECK(mliMap_has(&map, "hans"));
        CHECK(mliMap_get(&map, "hans", &ret));
        CHECK(ret == 1337);

        CHECK(mliMap_malloc_insert(&map, "peter", 42));
        CHECK(mliMap_has(&map, "peter"));
        CHECK(mliMap_get(&map, "peter", &ret));
        CHECK(ret == 42);

        CHECK(mliMap_malloc_insert(&map, "karl", 37));
        CHECK(mliMap_has(&map, "karl"));
        CHECK(mliMap_get(&map, "karl", &ret));
        CHECK(ret == 37);

        CHECK(mliMap_malloc_insert(&map, "karl", 37) == 1);

        mliMap_free(&map);
}

CASE("map2") {
        uint64_t i;
        struct mliMT19937 prng = mliMT19937_init(0);
        struct mliMap map = mliMap_init();

        for (i = 0; i < 1000*100; i++) {
                char key_str[1024];
                uint64_t value = (uint64_t)mliMT19937_uint32(&prng);
                sprintf(key_str, "%lu", value);

                if (!mliMap_has(&map, key_str)) {
                        CHECK(mliMap_malloc_insert(&map, key_str, value));
                }
        }

        mliMap_free(&map);
}
