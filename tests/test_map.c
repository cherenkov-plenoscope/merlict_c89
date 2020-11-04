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

CASE("map3")
{
        uint64_t ret = 0;
        struct mliDynMap3 map = mliDynMap3_init();
        CHECK(mliDynMap3_malloc(&map, 0u));

        CHECK(mliDynMap3_has(&map, "hans") == 0);
        CHECK(mliDynMap3_insert(&map, "hans", 1337));
        CHECK(mliDynMap3_has(&map, "hans"));
        CHECK(mliDynMap3_get(&map, "hans", &ret));
        CHECK(ret == 1337);

        CHECK(mliDynMap3_insert(&map, "peter", 42));
        CHECK(mliDynMap3_has(&map, "peter"));
        CHECK(mliDynMap3_get(&map, "peter", &ret));
        CHECK(ret == 42);

        CHECK(mliDynMap3_insert(&map, "karl", 37));
        CHECK(mliDynMap3_has(&map, "karl"));
        CHECK(mliDynMap3_get(&map, "karl", &ret));
        CHECK(ret == 37);

        CHECK(!mliDynMap3_insert(&map, "karl", 37));

        CHECK(map.dyn.size == 3u);
        mliDynMap3_free(&map);
}
