/* Copyright 2019-2020 Sebastian Achim Mueller                                */

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

        CHECK(map.size == 4u);
        mliMap_free(&map);
}

CASE("map2") {
        uint64_t i;
        struct mliMT19937 prng = mliMT19937_init(0);
        struct mliMap map = mliMap_init();

        for (i = 0; i < 1000; i++) {
                char key_str[1024];
                uint64_t value = (uint64_t)(mli_random_uniform(&prng)*(1000));
                sprintf(key_str, "%lu", value);

                if (!mliMap_has(&map, key_str)) {
                        CHECK(mliMap_malloc_insert(&map, key_str, value));
                }
        }

        mliMap_free(&map);
}

CASE("empty_map") {
        struct mliMap map = mliMap_init();
        CHECK(map.size == 0u);
        mliMap_free(&map);
}
