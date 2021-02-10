/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery_tar.h"
#include "mliScenery_valid.h"
#include <math.h>
#include <stdint.h>
#include "mli_debug.h"

int mliScenery_malloc_from_tar(
        struct mliScenery *scenery,
        const char *path)
{
        struct mliArchive archive = mliArchive_init();

        mli_check(
                mliArchive_malloc_from_tar(&archive, path),
                "Can not read tape-archive to malloc mliScenery.");

        mli_check(
                mliScenery_malloc_from_Archive(scenery, &archive),
                "Can not malloc mliUsergeometry from archive.");

        mliArchive_free(&archive);

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

        mli_check(mli_malloc_materials_form_archive(
                &scenery->materials,
                &material_names,
                archive),
                "Failed to malloc materials.");

        mli_c(mliDynMap_malloc(&object_names, 0u));

        num_objects = mliArchive_num_filename_prefix_sufix(
                archive,
                "objects/",
                ".obj");

        mli_check(_mliGeometry_malloc_objects(
                &scenery->geometry,
                num_objects),
                "Failed to malloc geometry.objects.");

        mli_check(mli_set_geometry_objects_and_names_from_archive(
                &scenery->geometry,
                &object_names,
                archive),
                "Failed to malloc geometry.objects.");

        mli_check(
                mli_malloc_root_frame_from_Archive(
                        &root,
                        archive,
                        &object_names,
                        scenery->geometry.objects,
                        &material_names.boundary_layers),
                "Failed to malloc and populate tree of frames.");

        mli_check(
                mliFrame_estimate_num_robjects_and_total_num_boundary_layers(
                        &root,
                        &num_robjects,
                        &total_num_boundary_layers),
                "Can not estimate num_robjects from tree of frames.");

        mli_check(
                mliGeometryToMaterialMap_malloc(
                        &scenery->geomap,
                        num_robjects,
                        total_num_boundary_layers),
                "Failed to malloc geometry to materials map.");

        mli_check(
                _mliGeometry_malloc_references(
                        &scenery->geometry,
                        num_robjects),
                "Failed to malloc geometry.references.");

        mli_check(
                mliFrame_set_robjects_and_material_map(
                        &root,
                        &scenery->geometry,
                        &scenery->geomap),
                "Can not set robjects.");

        mliNameMap_free(&material_names);
        mliDynMap_free(&object_names);
        mliFrame_free(&root);

        mli_check(mliAccelerator_malloc_from_Geometry(
                &scenery->accelerator,
                &scenery->geometry),
                "Failed to malloc accelerator from geometry.");

        mli_check(
                mliScenery_valid(scenery), "Expected scenery to be valid.");
        return 1;
error:
        return 0;
}
