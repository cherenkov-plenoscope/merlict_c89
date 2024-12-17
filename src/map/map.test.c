/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("vmap")
{
        uint64_t ptr;
        uint64_t ptr_back;
        struct mli_String hans = mli_String_init();
        struct mli_String peter = mli_String_init();
        struct mli_String karl = mli_String_init();

        struct mli_Map map = mli_Map_init();

        CHECK(mli_String_from_cstr(&hans, "hans"));
        CHECK(mli_String_from_cstr(&peter, "peter"));
        CHECK(mli_String_from_cstr(&karl, "karl"));

        CHECK(mli_Map_malloc(&map));

        CHECK(mli_Map_has(&map, &hans) == 0);
        ptr = 1337;
        CHECK(mli_Map_insert(&map, &hans, ptr));
        CHECK(mli_Map_has(&map, &hans));
        CHECK(mli_Map_get(&map, &hans, &ptr_back));
        CHECK(ptr_back == 1337);

        ptr = 42;
        CHECK(mli_Map_insert(&map, &peter, ptr));
        CHECK(mli_Map_has(&map, &peter));
        CHECK(mli_Map_get(&map, &peter, &ptr_back));
        CHECK(ptr_back == 42);

        ptr = 37;
        CHECK(mli_Map_insert(&map, &karl, ptr));
        CHECK(mli_Map_has(&map, &karl));
        CHECK(mli_Map_get(&map, &karl, &ptr_back));
        CHECK(ptr_back == 37);

        mli_String_free(&hans);
        mli_String_free(&peter);
        mli_String_free(&karl);
        mli_Map_free(&map);
}
