/* Copyright 2019-2020 Sebastian Achim Mueller                                */

/*
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

        CHECK(mliDynMap_has(&arc.objects, "teapot2.obj"));
        CHECK(mliDynMap_has(&arc.functions, "refractive_index_water.csv"));

        CHECK(
                mliDynMap_get(
                        &arc.objects,
                        "hexagonal_mirror_facet.obj",
                        (void **)&obj
                )
        );

        CHECK(obj->num_faces == 600);
        CHECK(obj->num_vertices == 331);
        CHECK(obj->num_vertex_normals == 331);

        mliArc_free(&arc);
}
*/

CASE("vmap")
{
        uint64_t ptr;
        uint64_t ptr_back;
        struct mliDynMap map = mliDynMap_init();
        CHECK(mliDynMap_malloc(&map, 0u));

        CHECK(mliDynMap_has(&map, "hans") == 0);
        ptr = 1337;
        CHECK(mliDynMap_insert(&map, "hans", ptr));
        CHECK(mliDynMap_has(&map, "hans"));
        CHECK(mliDynMap_get(&map, "hans", &ptr_back));
        CHECK(ptr_back == 1337);

        ptr = 42;
        CHECK(mliDynMap_insert(&map, "peter", ptr));
        CHECK(mliDynMap_has(&map, "peter"));
        CHECK(mliDynMap_get(&map, "peter", &ptr_back));
        CHECK(ptr_back == 42);

        ptr = 37;
        CHECK(mliDynMap_insert(&map, "karl", ptr));
        CHECK(mliDynMap_has(&map, "karl"));
        CHECK(mliDynMap_get(&map, "karl", &ptr_back));
        CHECK(ptr_back == 37);

        mliDynMap_free(&map);
}
