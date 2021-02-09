/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mliUserScenery, init")
{
        struct mliSceneryResources resources = mliSceneryResources_init();
        CHECK(resources.num_functions == 0u);
        CHECK(resources.functions == NULL);
        CHECK(resources.num_colors == 0u);
        CHECK(resources.colors == NULL);
        CHECK(resources.num_surfaces == 0u);
        CHECK(resources.surfaces == NULL);
        CHECK(resources.num_media == 0u);
        CHECK(resources.media == NULL);
}

CASE("mliSceneryResources, estimate capacity from json")
{
        char json_str[256];
        struct mliJson material_json = mliJson_init();
        struct mliSceneryResourcesCapacity rescap =
                mliSceneryResourcesCapacity_init();

        sprintf(
                json_str,
                "{\n"
                "    \"colors\": [1, 2],\n"
                "    \"surfaces\": [1, 2, 3, 4, 5, 6, 7, 8]\n"
                "    \"media\": [1],\n"
                "}\n"
        );
        CHECK(mliJson_malloc_from_string(&material_json, json_str));
        CHECK(__mliSceneryResourcesCapacity_from_materials_json(
                &rescap,
                &material_json));

        CHECK(rescap.num_colors == 2);
        CHECK(rescap.num_surfaces == 8);
        CHECK(rescap.num_media == 1);

        mliJson_free(&material_json);
}

/*
CASE("mliCombine, malloc from archive")
{
        struct mliCombine combine = mliCombine_init();
        struct mliFrame *_root, *_obj1, *_frame2, *_obj3, *_obj4 = NULL;

        uint64_t obj_idx, obj_teapot_idx, obj_hex_idx;
        uint64_t srf_grass, srf_wood, srf_leafs, srf_blue_glass;
        uint64_t med_vacuum;
        uint64_t fcn_idx;

        CHECK(mliCombine_malloc_from_tar(
                &combine,
                "tests/"
                "resources/"
                "sceneries/"
                "001.tar"
        ));

        CHECK(2 == combine.scenery.num_objects);

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

        /* frames
         * ------
         *
         *      root
         *      |______ obj1 (teapot)
         *      |
         *      |______ frame2
         *              |_____ obj3 (hex)
         *              |
         *              |_____ obj4 (hex)
         *

        CHECK(mliDynMap_get(
                &uscn.object_names,
                "teapot",
                &obj_teapot_idx));

        CHECK(mliDynMap_get(
                &uscn.object_names,
                "hexagonal_mirror_facet",
                &obj_hex_idx));

        CHECK(mliDynMap_get(&uscn.surface_names, "grass", &srf_grass));
        CHECK(mliDynMap_get(&uscn.surface_names, "wood", &srf_wood));
        CHECK(mliDynMap_get(&uscn.surface_names, "leafs", &srf_leafs));
        CHECK(mliDynMap_get(&uscn.surface_names, "blue_glass", &srf_blue_glass));
        CHECK(mliDynMap_get(&uscn.medium_names, "vacuum", &med_vacuum));

        /* root *
        _root = &uscn.root;
        CHECK(_root->id == 0);
        CHECK(_root->type == MLI_FRAME);
        CHECK(_root->children.dyn.size == 2);

        /* obj1 *
        _obj1 = _root->children.arr[0];
        CHECK(_obj1->type == MLI_OBJECT);
        CHECK(_obj1->id == 1);
        CHECK(_obj1->object == obj_teapot_idx);
        CHECK(_obj1->boundary_layer.inner.surface == srf_wood);
        CHECK(_obj1->boundary_layer.outer.surface == srf_blue_glass);
        CHECK(_obj1->boundary_layer.inner.medium == med_vacuum);
        CHECK(_obj1->boundary_layer.outer.medium == med_vacuum);

        /* frame2 *
        _frame2 = _root->children.arr[1];
        CHECK(_frame2->type == MLI_FRAME);
        CHECK(_frame2->id == 2);
        CHECK(_frame2->children.dyn.size == 2);

        /* obj3 *
        _obj3 = _frame2->children.arr[0];
        CHECK(_obj3->type == MLI_OBJECT);
        CHECK(_obj3->id == 3);
        CHECK(_obj3->object == obj_hex_idx);
        CHECK(_obj3->boundary_layer.inner.surface == srf_leafs);
        CHECK(_obj3->boundary_layer.outer.surface == srf_wood);
        CHECK(_obj3->boundary_layer.inner.medium == med_vacuum);
        CHECK(_obj3->boundary_layer.outer.medium == med_vacuum);

        /* obj4 *
        _obj4 = _frame2->children.arr[1];
        CHECK(_obj4->type == MLI_OBJECT);
        CHECK(_obj4->id == 4);
        CHECK(_obj4->object == obj_hex_idx);
        CHECK(_obj4->boundary_layer.inner.surface == srf_grass);
        CHECK(_obj4->boundary_layer.outer.surface == srf_wood);
        CHECK(_obj4->boundary_layer.inner.medium == med_vacuum);
        CHECK(_obj4->boundary_layer.outer.medium == med_vacuum);

        mliUserScenery_free(&uscn);
}
*/

/*
CASE("mliUserScenery, read, write mliSceneryResources")
{
        struct mliUserScenery uscn = mliUserScenery_init();
        struct mliSceneryResourcesCapacity rescap =
                mliSceneryResourcesCapacity_init();
        struct mliSceneryResources resources = mliSceneryResources_init();
        FILE *f = NULL;

        CHECK(mliUserScenery_malloc_from_tar(
                &uscn,
                "tests/"
                "resources/"
                "sceneries/"
                "001.tar"
        ));

        f = fopen("tests/resources/sceneryresources.bin.tmp", "w");
        CHECK(f != NULL);
        CHECK(mliSceneryResources_capacity_fwrite(&uscn.resources, f));
        CHECK(mliSceneryResources_fwrite(&uscn.resources, f));
        fclose(f);

        f = fopen("tests/resources/sceneryresources.bin.tmp", "r");
        CHECK(f != NULL);
        CHECK(mliSceneryResourcesCapacity_fread(&rescap, f));
        CHECK(mliSceneryResources_malloc(&resources, rescap));
        CHECK(mliSceneryResources_malloc_fread(&resources, f));
        fclose(f);

        CHECK(mliSceneryResources_equal(&resources, &uscn.resources));

        mliUserScenery_free(&uscn);
}
*/