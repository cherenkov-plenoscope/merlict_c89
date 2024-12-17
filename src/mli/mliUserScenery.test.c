/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mliUserScenery, init")
{
        struct mliMaterials resources = mliMaterials_init();
        CHECK(resources.num_surfaces == 0u);
        CHECK(resources.surfaces == NULL);
        CHECK(resources.num_media == 0u);
        CHECK(resources.media == NULL);
}

CASE("mliMaterials, estimate capacity from archive") {}

CASE("mliScenery, malloc from archive")
{
        struct mliScenery scenery = mliScenery_init();

        uint64_t obj_teapot_idx, obj_hex_idx;
        uint64_t srf_grass, srf_wood, srf_leafs, srf_blue_glass;
        uint64_t med_vacuum;

        CHECK(mliScenery_malloc_from_path_tar(
                &scenery,
                "data/"
                "mli/"
                "tests/"
                "resources/"
                "sceneries/"
                "001.tar"));

        CHECK(2 == scenery.geometry.num_objects);

        CHECK(mli_String__find_idx_with_cstr(
                scenery.geometry.object_names,
                scenery.geometry.num_objects,
                "hexagonal_mirror_facet",
                &obj_hex_idx));
        CHECK(600 == scenery.geometry.objects[obj_hex_idx].num_faces);
        CHECK(331 == scenery.geometry.objects[obj_hex_idx].num_vertices);
        CHECK(331 == scenery.geometry.objects[obj_hex_idx].num_vertex_normals);

        CHECK(mli_String__find_idx_with_cstr(
                scenery.geometry.object_names,
                scenery.geometry.num_objects,
                "teapot",
                &obj_teapot_idx));
        CHECK(2256 == scenery.geometry.objects[obj_teapot_idx].num_faces);
        CHECK(1202 == scenery.geometry.objects[obj_teapot_idx].num_vertices);
        CHECK(1202 ==
              scenery.geometry.objects[obj_teapot_idx].num_vertex_normals);

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

        CHECK(mli_String__find_idx_with_cstr(
                scenery.materials.surface_names,
                scenery.materials.num_surfaces,
                "grass",
                &srf_grass));
        CHECK(mli_String__find_idx_with_cstr(
                scenery.materials.surface_names,
                scenery.materials.num_surfaces,
                "wood",
                &srf_wood));
        CHECK(mli_String__find_idx_with_cstr(
                scenery.materials.surface_names,
                scenery.materials.num_surfaces,
                "leafs",
                &srf_leafs));
        CHECK(mli_String__find_idx_with_cstr(
                scenery.materials.surface_names,
                scenery.materials.num_surfaces,
                "blue_glass",
                &srf_blue_glass));
        CHECK(mli_String__find_idx_with_cstr(
                scenery.materials.medium_names,
                scenery.materials.num_media,
                "vacuum",
                &med_vacuum));

        mliScenery_free(&scenery);
}

CASE("mliScenery, read, write")
{
        struct mliScenery orig = mliScenery_init();
        struct mliScenery back = mliScenery_init();
        struct mli_IO f = mli_IO_init();
        char geom_path[] = "data/mli/tests/resources/geometry.bin.tmp";
        char mate_path[] = "data/mli/tests/resources/materials.bin.tmp";
        char acce_path[] = "data/mli/tests/resources/accelerator.bin.tmp";
        char gmap_path[] = "data/mli/tests/resources/geomap.bin.tmp";

        CHECK(mliScenery_malloc_from_path_tar(
                &orig,
                "data/"
                "mli/"
                "tests/"
                "resources/"
                "sceneries/"
                "002.tar"));

        /* geometry */
        /* -------- */
        CHECK(mli_IO__open_file_cstr(&f, geom_path, "w"));
        CHECK(mliGeometry_fwrite(&orig.geometry, &f));
        mli_IO_close(&f);

        CHECK(mli_IO__open_file_cstr(&f, geom_path, "r"));
        CHECK(mliGeometry_malloc_fread(&back.geometry, &f));
        mli_IO_close(&f);

        CHECK(mliGeometry_equal(&back.geometry, &orig.geometry));

        /* materials */
        /* --------- */
        CHECK(mli_IO__open_file_cstr(&f, mate_path, "w"));
        CHECK(mliMaterials_fwrite(&orig.materials, &f));
        mli_IO_close(&f);

        CHECK(mli_IO__open_file_cstr(&f, mate_path, "r"));
        CHECK(mliMaterials_malloc_fread(&back.materials, &f));
        mli_IO_close(&f);

        CHECK(mliMaterials_equal(&back.materials, &orig.materials));

        /* accelerator */
        /* ----------- */
        CHECK(mli_IO__open_file_cstr(&f, acce_path, "w"));
        CHECK(mliAccelerator_fwrite(&orig.accelerator, &f));
        mli_IO_close(&f);

        CHECK(mli_IO__open_file_cstr(&f, acce_path, "r"));
        CHECK(mliAccelerator_malloc_fread(&back.accelerator, &f));
        mli_IO_close(&f);

        CHECK(mliAccelerator_equal(&back.accelerator, &orig.accelerator));

        /* geomap */
        /* ------ */
        CHECK(mli_IO__open_file_cstr(&f, gmap_path, "w"));
        CHECK(mliGeometryToMaterialMap_fwrite(&orig.geomap, &f));
        mli_IO_close(&f);

        CHECK(mli_IO__open_file_cstr(&f, gmap_path, "r"));
        CHECK(mliGeometryToMaterialMap_malloc_fread(&back.geomap, &f));
        mli_IO_close(&f);

        CHECK(mliGeometryToMaterialMap_equal(&back.geomap, &orig.geomap));

        /* full scenery */
        /* ------------ */
        CHECK(mliScenery_write_to_path(
                &orig, "data/mli/tests/resources/scenery.bin.tmp"));
        CHECK(mliScenery_malloc_from_path(
                &back, "data/mli/tests/resources/scenery.bin.tmp"));
        CHECK(mliScenery_equal(&back, &orig));

        mliScenery_free(&back);
        mliScenery_free(&orig);
}
