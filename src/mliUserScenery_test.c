/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "mli_testing.h"
#include "mliUserScenery.h"

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

        fprintf(stderr, "%s, %d\n", __FILE__, __LINE__);
        CHECK(mliScenery_malloc_from_path_tar(
                &scenery,
                "tests/"
                "resources/"
                "sceneries/"
                "001.tar"));
        fprintf(stderr, "%s, %d\n", __FILE__, __LINE__);

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
        struct mliScenery orig = mliScenery_init();
        struct mliScenery back = mliScenery_init();
        FILE *f = NULL;

        CHECK(mliScenery_malloc_from_path_tar(
                &orig,
                "tests/"
                "resources/"
                "sceneries/"
                "002.tar"));

        /* geometry */
        /* -------- */
        f = fopen("tests/resources/geometry.bin.tmp", "w");
        CHECK(f != NULL);
        CHECK(mliGeometry_fwrite(&orig.geometry, f));
        fclose(f);
        f = fopen("tests/resources/geometry.bin.tmp", "r");
        CHECK(f != NULL);
        CHECK(mliGeometry_malloc_fread(&back.geometry, f));
        fclose(f);
        CHECK(mliGeometry_equal(&back.geometry, &orig.geometry));

        /* materials */
        /* --------- */
        f = fopen("tests/resources/materials.bin.tmp", "w");
        CHECK(f != NULL);
        CHECK(mliMaterials_fwrite(&orig.materials, f));
        fclose(f);
        f = fopen("tests/resources/materials.bin.tmp", "r");
        CHECK(f != NULL);
        CHECK(mliMaterials_malloc_fread(&back.materials, f));
        fclose(f);
        CHECK(mliMaterials_equal(&back.materials, &orig.materials));

        /* accelerator */
        /* ----------- */
        f = fopen("tests/resources/accelerator.bin.tmp", "w");
        CHECK(f != NULL);
        CHECK(mliAccelerator_fwrite(&orig.accelerator, f));
        fclose(f);
        f = fopen("tests/resources/accelerator.bin.tmp", "r");
        CHECK(f != NULL);
        CHECK(mliAccelerator_malloc_fread(&back.accelerator, f));
        fclose(f);
        CHECK(mliAccelerator_equal(&back.accelerator, &orig.accelerator));

        /* geomap */
        /* ------ */
        f = fopen("tests/resources/geomap.bin.tmp", "w");
        CHECK(f != NULL);
        CHECK(mliGeometryToMaterialMap_fwrite(&orig.geomap, f));
        fclose(f);
        f = fopen("tests/resources/geomap.bin.tmp", "r");
        CHECK(f != NULL);
        CHECK(mliGeometryToMaterialMap_malloc_fread(&back.geomap, f));
        fclose(f);
        CHECK(mliGeometryToMaterialMap_equal(&back.geomap, &orig.geomap));

        /* full scenery */
        /* ------------ */
        CHECK(mliScenery_write_to_path(
                &orig, "tests/resources/scenery.bin.tmp"));
        CHECK(mliScenery_malloc_from_path(
                &back, "tests/resources/scenery.bin.tmp"));
        CHECK(mliScenery_equal(&back, &orig));

        mliScenery_free(&back);
        mliScenery_free(&orig);
}
