/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("mli_Scenery, malloc from archive")
{
        struct mli_Scenery scenery = mli_Scenery_init();

        uint64_t obj_teapot_idx, obj_hex_idx;

        CHECK(mli_Scenery__from_path_cstr(
                &scenery,
                "data/"
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

        CHECK(4 == scenery.materials.surfaces.size);
        CHECK(2 == scenery.materials.media.size);

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

        CHECK(mli_SurfaceArray_has_name_cstr(
                &scenery.materials.surfaces, "grass"));
        CHECK(mli_SurfaceArray_has_name_cstr(
                &scenery.materials.surfaces, "wood"));
        CHECK(mli_SurfaceArray_has_name_cstr(
                &scenery.materials.surfaces, "leafs"));
        CHECK(mli_SurfaceArray_has_name_cstr(
                &scenery.materials.surfaces, "blue_glass"));

        CHECK(mli_MediumArray_has_name_cstr(
                &scenery.materials.media, "vacuum"));

        mli_Scenery_free(&scenery);
}

CASE("mli_Scenery, read, write")
{
        struct mli_Scenery orig = mli_Scenery_init();
        struct mli_Scenery back = mli_Scenery_init();
        struct mli_IO f = mli_IO_init();
        char geom_path[] = "data/geometry.bin.tmp";
        char mate_path[] = "data/materials.bin.tmp";
        char acce_path[] = "data/accelerator.bin.tmp";
        char gmap_path[] = "data/geomap.bin.tmp";

        CHECK(mli_Scenery__from_path_cstr(
                &orig,
                "data/"
                "sceneries/"
                "002.tar"));

        /* geometry */
        /* -------- */
        CHECK(mli_IO_open_file_cstr(&f, geom_path, "w"));
        CHECK(mli_Geometry_to_io(&orig.geometry, &f));
        mli_IO_close(&f);

        CHECK(mli_IO_open_file_cstr(&f, geom_path, "r"));
        CHECK(mli_Geometry_from_io(&back.geometry, &f));
        mli_IO_close(&f);

        CHECK(mli_Geometry_equal(&back.geometry, &orig.geometry));

        /* materials */
        /* --------- */
        CHECK(mli_IO_open_file_cstr(&f, mate_path, "w"));
        CHECK(mli_Materials_to_io(&orig.materials, &f));
        mli_IO_close(&f);

        CHECK(mli_IO_open_file_cstr(&f, mate_path, "r"));
        CHECK(mli_Materials_from_io(&back.materials, &f));
        mli_IO_close(&f);

        CHECK(mli_Materials_equal(&back.materials, &orig.materials));

        /* accelerator */
        /* ----------- */
        CHECK(mli_IO_open_file_cstr(&f, acce_path, "w"));
        CHECK(mli_Accelerator_to_io(&orig.accelerator, &f));
        mli_IO_close(&f);

        CHECK(mli_IO_open_file_cstr(&f, acce_path, "r"));
        CHECK(mli_Accelerator_from_io(&back.accelerator, &f));
        mli_IO_close(&f);

        CHECK(mli_Accelerator_equal(&back.accelerator, &orig.accelerator));

        /* geomap */
        /* ------ */
        CHECK(mli_IO_open_file_cstr(&f, gmap_path, "w"));
        CHECK(mli_GeometryToMaterialMap_to_io(&orig.geomap, &f));
        mli_IO_close(&f);

        CHECK(mli_IO_open_file_cstr(&f, gmap_path, "r"));
        CHECK(mli_GeometryToMaterialMap_from_io(&back.geomap, &f));
        mli_IO_close(&f);

        CHECK(mli_GeometryToMaterialMap_equal(&back.geomap, &orig.geomap));

        /* full scenery */
        /* ------------ */
        CHECK(mli_Scenery_write_to_path(&orig, "data/scenery.bin.tmp"));
        CHECK(mli_Scenery_malloc_from_path(&back, "data/scenery.bin.tmp"));
        CHECK(mli_Scenery_equal(&back, &orig));

        mli_Scenery_free(&back);
        mli_Scenery_free(&orig);
}
