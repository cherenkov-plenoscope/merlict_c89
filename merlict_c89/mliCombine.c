/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliCombine.h"
#include <math.h>
#include <stdint.h>
#include "mli_debug.h"

struct mliCombine mliCombine_init(void)
{
        struct mliCombine combine;
        combine.geometry = mliGeometry_init();
        combine.accelerator = mliAccelerator_init();
        combine.materials = mliMaterials_init();
        return combine;
}

void mliCombine_free(struct mliCombine *combine)
{
        mliGeometry_free(&combine->geometry);
        mliAccelerator_free(&combine->accelerator);
        mliMaterials_free(&combine->materials);
}

int mliCombine_valid(const struct mliCombine *combine)
{
        mli_check(
                mliMaterials_valid(&combine->materials),
                "Expected materials to be valid.");

        mli_check(
                mliGeometry_valid(&combine->geometry, &combine->materials),
                "Expected geometry to be valid.");


        return 1;
error:
        return 0;
}

int mliCombine_malloc_from_tar(
        struct mliCombine *combine,
        const char *path)
{
        struct mliArchive archive = mliArchive_init();

        mli_check(
                mliArchive_malloc_from_tar(&archive, path),
                "Can not read tape-archive to malloc mliCombine.");

        mli_check(
                mliCombine_malloc_from_Archive(combine, &archive),
                "Can not malloc mliUsergeometry from archive.");

        mliArchive_free(&archive);

        return 1;
error:
        return 0;
}

int mliCombine_malloc_from_Archive(
        struct mliCombine *combine,
        const struct mliArchive *archive)
{
        uint64_t num_robjects = 0u;
        uint64_t num_objects = 0u;
        uint64_t robject_counter = 0u;

        struct mliNameMap material_names = mliNameMap_init();
        struct mliDynMap object_names = mliDynMap_init();
        struct mliFrame root = mliFrame_init();

        fprintf(stderr, "%s, %d\n", __FILE__, __LINE__);
        mli_check(mli_malloc_materials_form_archive(
                &combine->materials,
                &material_names,
                archive),
                "Failed to malloc materials.");

        fprintf(stderr, "%s, %d\n", __FILE__, __LINE__);
        mli_check(mli_malloc_object_names_from_archive(
                &object_names,
                archive),
                "Failed to malloc object-names.");

        fprintf(stderr, "%s, %d\n", __FILE__, __LINE__);
        mli_check(
                mli_malloc_root_frame_from_Archive(
                        &root,
                        archive,
                        &object_names,
                        &material_names.surfaces,
                        &material_names.media),
                "Failed to malloc and populate tree of frames.");

        fprintf(stderr, "%s, %d\n", __FILE__, __LINE__);
        mli_check(
                __mliGeometry_estimate_num_robjects_in_frames(
                        &num_robjects, &root),
                "Can not estimate num_robjects from tree of frames.");
        num_objects = object_names.dyn.size;

        fprintf(stderr, "%s, %d\n", __FILE__, __LINE__);
        mli_check(
                mliGeometry_malloc(
                        &combine->geometry,
                        num_objects,
                        num_robjects),
                "Failed to malloc geometry.");

        fprintf(stderr, "%s, %d\n", __FILE__, __LINE__);
        mli_check(
                mliGeometry_set_objects_from_Archive(
                        &combine->geometry,
                        &object_names,
                        archive),
                "Failed to set objects in geometry from archive.");

        fprintf(stderr, "%s, %d\n", __FILE__, __LINE__);
        mli_check(
                __mliGeometry_set_robjects(
                        &combine->geometry, &root, &robject_counter),
                "Can not set robjects.");

        fprintf(stderr, "%s, %d\n", __FILE__, __LINE__);
        mli_check(
                num_robjects == robject_counter,
                "Expected to set the num_robjects found while "
                "estimating capacity.");

        fprintf(stderr, "%s, %d\n", __FILE__, __LINE__);
        mliNameMap_free(&material_names);
        mliDynMap_free(&object_names);
        mliFrame_free(&root);

        fprintf(stderr, "%s, %d\n", __FILE__, __LINE__);
        mli_check(mliAccelerator_malloc_from_Geometry(
                &combine->accelerator,
                &combine->geometry),
                "");

        fprintf(stderr, "%s, %d\n", __FILE__, __LINE__);
        mli_check(
                mliCombine_valid(combine), "Expected combine to be valid.");
        fprintf(stderr, "%s, %d\n", __FILE__, __LINE__);
        return 1;
error:
        return 0;
}
