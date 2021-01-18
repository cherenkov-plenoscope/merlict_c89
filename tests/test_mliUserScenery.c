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

CASE("mliUserScenery, malloc from archive")
{
        struct mliUserScenery uscn = mliUserScenery_init();
        uint64_t obj_idx;
        uint64_t fcn_idx;

        CHECK(mliUserScenery_malloc_from_tape_archive(
                &uscn,
                "tests/"
                "resources/"
                "sceneries/"
                "001.tar"
        ));

        CHECK(2 == uscn.resources.num_objects);

        CHECK(mliDynMap_get(
                &uscn.object_names,
                "hexagonal_mirror_facet",
                &obj_idx));
        CHECK(600 == uscn.resources.objects[obj_idx].num_faces);
        CHECK(331 == uscn.resources.objects[obj_idx].num_vertices);
        CHECK(331 == uscn.resources.objects[obj_idx].num_vertex_normals);

        CHECK(mliDynMap_get(
                &uscn.object_names,
                "teapot",
                &obj_idx));
        CHECK(2256 == uscn.resources.objects[obj_idx].num_faces);
        CHECK(1202 == uscn.resources.objects[obj_idx].num_vertices);
        CHECK(1202 == uscn.resources.objects[obj_idx].num_vertex_normals);

        CHECK(2 == uscn.resources.num_functions);

        CHECK(mliDynMap_get(
                &uscn.function_names,
                "refractive_index_water",
                &fcn_idx));
        CHECK(mliDynMap_get(
                &uscn.function_names,
                "zero",
                &fcn_idx));

        CHECK(4 == uscn.resources.num_colors);
        CHECK(4 == uscn.resources.num_surfaces);
        CHECK(2 == uscn.resources.num_media);

        /* frames */

        CHECK(0 == uscn.root.id);
        CHECK(MLI_FRAME == uscn.root.type);
        CHECK(2 == uscn.root.children.dyn.size);

        CHECK(MLI_OBJECT == uscn.root.children.arr[0]->type);
        CHECK(1 == uscn.root.children.arr[0]->id);

        CHECK(mliDynMap_get(
                &uscn.object_names,
                "teapot",
                &obj_idx));
        CHECK(obj_idx == uscn.root.children.arr[0]->object);

        CHECK(MLI_FRAME == uscn.root.children.arr[1]->type);
        CHECK(2 == uscn.root.children.arr[1]->id);

        mliUserScenery_free(&uscn);
}
