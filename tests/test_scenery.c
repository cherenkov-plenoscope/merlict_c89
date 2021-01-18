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
        CHECK(scenery.robject_user_ids == NULL);
        CHECK(scenery.robject_boundary_layers == NULL);
        CHECK(scenery.robject2root == NULL);
}
