/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mliScenery, init")
{
        struct mliScenery scenery = mliScenery_init();
        CHECK(scenery.resources.num_objects == 0u);
        CHECK(scenery.resources.objects == NULL);

        CHECK(scenery.resources.num_functions == 0u);
        CHECK(scenery.resources.functions == NULL);

        CHECK(scenery.resources.num_colors == 0u);
        CHECK(scenery.resources.colors == NULL);

        CHECK(scenery.resources.num_surfaces == 0u);
        CHECK(scenery.resources.surfaces == NULL);

        CHECK(scenery.resources.num_media == 0u);
        CHECK(scenery.resources.media == NULL);

        CHECK(scenery.resources.default_medium == 0u);

        CHECK(scenery.num_robjects == 0u);
        CHECK(scenery.robjects == NULL);
        CHECK(scenery.robject_ids == NULL);
        CHECK(scenery.robject_boundary_layers == NULL);
        CHECK(scenery.robject2root == NULL);
}

CASE("mliScenery, malloc from mliUserScenery")
{
        struct mliScenery scn = mliScenery_init();
        struct mliScenery scn_back = mliScenery_init();
        struct mliUserScenery uscn = mliUserScenery_init();

        CHECK(mliUserScenery_malloc_from_tar(
                &uscn,
                "tests/"
                "resources/"
                "sceneries/"
                "001.tar"
        ));

        CHECK(mliScenery_malloc_from_mliUserScenery(&scn, &uscn));

        CHECK(mliSceneryResources_equal(&scn.resources, &uscn.resources));
        mliUserScenery_free(&uscn);

        CHECK(mliScenery_is_equal(&scn, &scn));

        CHECK(mliScenery_write_to_path(
                &scn,
                "tests/"
                "resources/"
                "sceneries/"
                "001.mli.tmp"
        ));

        CHECK(mliScenery_malloc_from_path(
                &scn_back,
                "tests/"
                "resources/"
                "sceneries/"
                "001.mli.tmp"
        ));

        CHECK(mliScenery_is_equal(&scn, &scn_back));

        mliScenery_free(&scn);
        mliScenery_free(&scn_back);
}
