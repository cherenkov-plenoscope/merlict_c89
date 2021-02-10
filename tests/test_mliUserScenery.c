/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mliUserScenery, init")
{
        struct mliMaterials resources = mliMaterials_init();
        CHECK(resources.num_functions == 0u);
        CHECK(resources.functions == NULL);
        CHECK(resources.num_colors == 0u);
        CHECK(resources.colors == NULL);
        CHECK(resources.num_surfaces == 0u);
        CHECK(resources.surfaces == NULL);
        CHECK(resources.num_media == 0u);
        CHECK(resources.media == NULL);
}

CASE("mliMaterials, estimate capacity from json")
{
        char json_str[256];
        struct mliJson material_json = mliJson_init();
        struct mliMaterialsCapacity rescap =
                mliMaterialsCapacity_init();

        sprintf(
                json_str,
                "{\n"
                "    \"colors\": [1, 2],\n"
                "    \"surfaces\": [1, 2, 3, 4, 5, 6, 7, 8]\n"
                "    \"media\": [1],\n"
                "    \"boundary_layers\": [1, 2, 3, 4],\n"
                "}\n"
        );
        CHECK(mliJson_malloc_from_string(&material_json, json_str));
        CHECK(__mliMaterialsCapacity_from_materials_json(
                &rescap,
                &material_json));

        CHECK(rescap.num_colors == 2);
        CHECK(rescap.num_surfaces == 8);
        CHECK(rescap.num_media == 1);
        CHECK(rescap.num_boundary_layers == 4);

        mliJson_free(&material_json);
}

CASE("mliScenery, malloc from archive")
{
        struct mliScenery scenery = mliScenery_init();

        uint64_t obj_teapot_idx, obj_hex_idx;
        uint64_t srf_grass, srf_wood, srf_leafs, srf_blue_glass;
        uint64_t med_vacuum;
        uint64_t fcn_idx;

        CHECK(mliScenery_malloc_from_tar(
                &scenery,
                "tests/"
                "resources/"
                "sceneries/"
                "001.tar"
        ));

        CHECK(2 == scenery.geometry.num_objects);

        CHECK(mliName_find_idx(
                scenery.geometry.object_names,
                scenery.geometry.num_objects,
                "hexagonal_mirror_facet",
                &obj_hex_idx));
        CHECK(600 == scenery.geometry.objects[obj_hex_idx].num_faces);
        CHECK(331 == scenery.geometry.objects[obj_hex_idx].num_vertices);
        CHECK(331 == scenery.geometry.objects[obj_hex_idx].num_vertex_normals);

        CHECK(mliName_find_idx(
                scenery.geometry.object_names,
                scenery.geometry.num_objects,
                "teapot",
                &obj_teapot_idx));
        CHECK(2256 == scenery.geometry.objects[obj_teapot_idx].num_faces);
        CHECK(1202 == scenery.geometry.objects[obj_teapot_idx].num_vertices);
        CHECK(1202 == scenery.geometry.objects[obj_teapot_idx].num_vertex_normals);

        CHECK(2 == scenery.materials.num_functions);

        CHECK(mliName_find_idx(
                scenery.materials.function_names,
                scenery.materials.num_functions,
                "refractive_index_water",
                &fcn_idx));
        CHECK(mliName_find_idx(
                scenery.materials.function_names,
                scenery.materials.num_functions,
                "zero",
                &fcn_idx));
        CHECK(4 == scenery.materials.num_colors);
        CHECK(4 == scenery.materials.num_surfaces);
        CHECK(2 == scenery.materials.num_media);

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
         */

        CHECK(mliName_find_idx(
                scenery.materials.surface_names,
                scenery.materials.num_surfaces,
                "grass",
                &srf_grass));
        CHECK(mliName_find_idx(
                scenery.materials.surface_names,
                scenery.materials.num_surfaces,
                "wood",
                &srf_wood));
        CHECK(mliName_find_idx(
                scenery.materials.surface_names,
                scenery.materials.num_surfaces,
                "leafs",
                &srf_leafs));
        CHECK(mliName_find_idx(
                scenery.materials.surface_names,
                scenery.materials.num_surfaces,
                "blue_glass",
                &srf_blue_glass));
        CHECK(mliName_find_idx(
                scenery.materials.medium_names,
                scenery.materials.num_media,
                "vacuum",
                &med_vacuum));

        mliScenery_free(&scenery);
}

CASE("mliScenery, read, write")
{
        struct mliScenery scenery = mliScenery_init();
        struct mliMaterials material_back = mliMaterials_init();
        FILE *f = NULL;

        CHECK(mliScenery_malloc_from_tar(
                &scenery,
                "tests/"
                "resources/"
                "sceneries/"
                "001.tar"
        ));

        f = fopen("tests/resources/materials.bin.tmp", "w");
        CHECK(f != NULL);
        CHECK(mliMaterials_fwrite(&scenery.materials, f));
        fclose(f);

        f = fopen("tests/resources/materials.bin.tmp", "r");
        CHECK(f != NULL);
        CHECK(mliMaterials_malloc_fread(&material_back, f));
        fclose(f);

        CHECK(mliMaterials_equal(&material_back, &scenery.materials));

        mliMaterials_free(&material_back);
        mliScenery_free(&scenery);
}
