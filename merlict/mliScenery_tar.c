/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery_tar.h"
#include <math.h>
#include <stdint.h>
#include "chk_debug.h"
#include "mliScenery_valid.h"
#include "mli_frame_to_scenery.h"

int mliScenery_malloc_fread_tar(struct mliScenery *scenery, FILE *f)
{
        struct mliArchive archive = mliArchive_init();
        chk_msg(mliArchive_malloc_fread(&archive, f),
                "Can't read archive from file.");
        chk_msg(mliScenery_malloc_from_Archive(scenery, &archive),
                "Can't malloc Scenery from Archive.");
        mliArchive_free(&archive);
        return 1;
error:
        return 0;
}

int mliScenery_malloc_from_path_tar(
        struct mliScenery *scenery,
        const char *path)
{
        FILE *f = fopen(path, "rb");
        chk_msgf(f != NULL, ("Can't open path '%s'.", path))
                chk_msg(mliScenery_malloc_fread_tar(scenery, f),
                        "Can't fread Scenery from file.");
        fclose(f);
        return 1;
error:
        return 0;
}

int mliScenery_malloc_from_Archive(
        struct mliScenery *scenery,
        const struct mliArchive *archive)
{
        uint64_t num_robjects = 0u;
        uint64_t num_objects = 0u;
        uint64_t total_num_boundary_layers = 0u;

        struct mliNameMap material_names = mliNameMap_init();
        struct mliDynMap object_names = mliDynMap_init();
        struct mliFrame root = mliFrame_init();

        chk_msg(mliMaterials_malloc_form_archive(
                        &scenery->materials, &material_names, archive),
                "Failed to malloc materials.");

        chk(mliDynMap_malloc(&object_names, 0u));

        num_objects = mliArchive_num_filename_prefix_sufix(
                archive, "geometry/objects/", ".obj");

        chk_msg(mliGeometry_malloc_objects(&scenery->geometry, num_objects),
                "Failed to malloc geometry.objects.");

        chk_msg(mli_set_geometry_objects_and_names_from_archive(
                        &scenery->geometry, &object_names, archive),
                "Failed to malloc geometry.objects.");

        chk_msg(mli_check_malloc_root_frame_from_Archive(
                        &root,
                        archive,
                        &object_names,
                        scenery->geometry.objects,
                        &material_names.boundary_layers),
                "Failed to malloc and populate tree of frames.");

        chk_msg(mliFrame_estimate_num_robjects_and_total_num_boundary_layers(
                        &root, &num_robjects, &total_num_boundary_layers),
                "Can not estimate num_robjects from tree of frames.");

        chk_msg(mliGeometryToMaterialMap_malloc(
                        &scenery->geomap,
                        num_robjects,
                        total_num_boundary_layers),
                "Failed to malloc geometry to materials map.");

        chk_msg(mliGeometry_malloc_references(&scenery->geometry, num_robjects),
                "Failed to malloc geometry.references.");

        chk_msg(mliFrame_set_robjects_and_material_map(
                        &root, &scenery->geometry, &scenery->geomap),
                "Can not set robjects.");

        mliNameMap_free(&material_names);
        mliDynMap_free(&object_names);
        mliFrame_free(&root);

        chk_msg(mliAccelerator_malloc_from_Geometry(
                        &scenery->accelerator, &scenery->geometry),
                "Failed to malloc accelerator from geometry.");

        chk_msg(mliScenery_valid(scenery), "Expected scenery to be valid.");
        chk_msg(mliGeometry_warn_objects(&scenery->geometry),
                "Failed to warn about objects.");

        return 1;
error:
        return 0;
}
