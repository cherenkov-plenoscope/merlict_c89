/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("arc")
{
        struct mliArc arc = mliArc_init();
        struct mliObject *obj = NULL;
        CHECK(
                mliArc_malloc_from_tar(
                        &arc,
                        "tests/resources/minimal_scenery.tar"
                )
        );

        CHECK(mliDynMapItem_has(&arc.objects, "teapot2.obj"));
        CHECK(mliDynMapItem_has(&arc.functions, "refractive_index_water.csv"));

        CHECK(
                mliDynMapItem_get(
                        &arc.objects,
                        "hexagonal_mirror_facet.obj",
                        (void *)&obj
                )
        );

        CHECK(obj->num_faces == 600);
        CHECK(obj->num_vertices == 331);
        CHECK(obj->num_vertex_normals == 331);

        mliArc_free(&arc);
}

CASE("vmap")
{
        void *ptr;
        void *ptr_back;
        struct mliDynMapItem map = mliDynMapItem_init();
        CHECK(mliDynMapItem_malloc(&map, 0u));

        CHECK(mliDynMapItem_has(&map, "hans") == 0);
        ptr = (void *)1337;
        CHECK(mliDynMapItem_malloc_insert(&map, "hans", ptr));
        CHECK(mliDynMapItem_has(&map, "hans"));
        CHECK(mliDynMapItem_get(&map, "hans", &ptr_back));
        CHECK(ptr_back == (void *)1337);

        ptr = (void *)42;
        CHECK(mliDynMapItem_malloc_insert(&map, "peter", ptr));
        CHECK(mliDynMapItem_has(&map, "peter"));
        CHECK(mliDynMapItem_get(&map, "peter", &ptr_back));
        CHECK(ptr_back == (void *)42);

        ptr = (void *)37;
        CHECK(mliDynMapItem_malloc_insert(&map, "karl", ptr));
        CHECK(mliDynMapItem_has(&map, "karl"));
        CHECK(mliDynMapItem_get(&map, "karl", &ptr_back));
        CHECK(ptr_back == (void *)37);

        mliDynMapItem_free(&map);
}

CASE("map")
{
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

CASE("map2")
{
        uint64_t i;
        struct mliMT19937 prng = mliMT19937_init(0);
        struct mliMap map = mliMap_init();

        for (i = 0; i < 1000; i++) {
                char key_str[1024];
                uint64_t value = (uint64_t)(mli_random_uniform(&prng) * (1000));
                sprintf(key_str, "%lu", value);

                if (!mliMap_has(&map, key_str)) {
                        CHECK(mliMap_malloc_insert(&map, key_str, value));
                }
        }

        mliMap_free(&map);
}

CASE("empty_map")
{
        struct mliMap map = mliMap_init();
        CHECK(map.size == 0u);
        mliMap_free(&map);
}
