/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery_tar.h"
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
        uint64_t robject_counter = 0u;

        struct mliNameMap material_names = mliNameMap_init();
        struct mliDynMap object_names = mliDynMap_init();
        struct mliFrame root = mliFrame_init();

        mli_check(mli_malloc_materials_form_archive(
                &scenery->materials,
                &material_names,
                archive),
                "Failed to malloc materials.");

        mli_check(mli_malloc_object_names_from_archive(
                &object_names,
                archive),
                "Failed to malloc object-names.");

        mli_check(
                mli_malloc_root_frame_from_Archive(
                        &root,
                        archive,
                        &object_names,
                        &material_names.surfaces,
                        &material_names.media),
                "Failed to malloc and populate tree of frames.");

        mli_check(
                __mliGeometry_estimate_num_robjects_in_frames(
                        &num_robjects, &root),
                "Can not estimate num_robjects from tree of frames.");
        num_objects = object_names.dyn.size;

        mli_check(
                mliGeometry_malloc(
                        &scenery->geometry,
                        num_objects,
                        num_robjects),
                "Failed to malloc geometry.");

        mli_check(
                mliGeometry_set_objects_from_Archive(
                        &scenery->geometry,
                        &object_names,
                        archive),
                "Failed to set objects in geometry from archive.");

        mli_check(
                __mliGeometry_set_robjects(
                        &scenery->geometry, &root, &robject_counter),
                "Can not set robjects.");

        mli_check(
                num_robjects == robject_counter,
                "Expected to set the num_robjects found while "
                "estimating capacity.");

        mliNameMap_free(&material_names);
        mliDynMap_free(&object_names);
        mliFrame_free(&root);

        mli_check(mliAccelerator_malloc_from_Geometry(
                &scenery->accelerator,
                &scenery->geometry),
                "");

        mli_check(
                mliScenery_valid(scenery), "Expected scenery to be valid.");
        return 1;
error:
        return 0;
}
