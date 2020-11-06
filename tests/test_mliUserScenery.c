/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mliUserScenery, init")
{
        struct mliSceneryResources resources = mliSceneryResources_init();
        CHECK(resources.num_objects == 0u);
        CHECK(resources.objects == NULL);
        CHECK(resources.num_functions == 0u);
        CHECK(resources.functions == NULL);
        CHECK(resources.num_colors == 0u);
        CHECK(resources.colors == NULL);
        CHECK(resources.num_surfaces == 0u);
        CHECK(resources.surfaces == NULL);
        CHECK(resources.num_media == 0u);
        CHECK(resources.media == NULL);
}

CASE("mliUserScenery, from json")
{
        char json_str[256];
        struct mliJson json = mliJson_init();
        struct mliSceneryResourcesCapacity uscncap =
                mliSceneryResourcesCapacity_init();

        sprintf(
                json_str,
                "{\n"
                "    \"objects\": [1, 2, 3, 4, 5],\n"
                "    \"functions\": [1, 2, 3],\n"
                "    \"colors\": [1, 2],\n"
                "    \"surfaces\": [1, 2, 3, 4, 5, 6, 7, 8]\n"
                "    \"media\": [1],\n"
                "}\n"
        );
        CHECK(mliJson_malloc_from_string(&json, json_str));
        CHECK(__mliSceneryResourcesCapacity_from_json(&uscncap, &json));

        CHECK(uscncap.num_objects == 5);
        CHECK(uscncap.num_functions == 3);
        CHECK(uscncap.num_colors == 2);
        CHECK(uscncap.num_surfaces == 8);
        CHECK(uscncap.num_media == 1);

        mliJson_free(&json);
}
