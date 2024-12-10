/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("vmap")
{
        uint64_t ptr;
        uint64_t ptr_back;
        struct mli_String hans = mli_String_init();
        struct mli_String peter = mli_String_init();
        struct mli_String karl = mli_String_init();

        struct mliDynMap map = mliDynMap_init();

        CHECK(mli_String_from_cstr(&hans, "hans"));
        CHECK(mli_String_from_cstr(&peter, "peter"));
        CHECK(mli_String_from_cstr(&karl, "karl"));

        CHECK(mliDynMap_malloc(&map));

        CHECK(mliDynMap_has(&map, &hans) == 0);
        ptr = 1337;
        CHECK(mliDynMap_insert(&map, &hans, ptr));
        CHECK(mliDynMap_has(&map, &hans));
        CHECK(mliDynMap_get(&map, &hans, &ptr_back));
        CHECK(ptr_back == 1337);

        ptr = 42;
        CHECK(mliDynMap_insert(&map, &peter, ptr));
        CHECK(mliDynMap_has(&map, &peter));
        CHECK(mliDynMap_get(&map, &peter, &ptr_back));
        CHECK(ptr_back == 42);

        ptr = 37;
        CHECK(mliDynMap_insert(&map, &karl, ptr));
        CHECK(mliDynMap_has(&map, &karl));
        CHECK(mliDynMap_get(&map, &karl, &ptr_back));
        CHECK(ptr_back == 37);

        mli_String_free(&hans);
        mli_String_free(&peter);
        mli_String_free(&karl);
        mliDynMap_free(&map);
}
