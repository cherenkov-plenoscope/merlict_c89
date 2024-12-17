/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery_tar.h"
#include <math.h>
#include <stdint.h>
#include "../chk/chk.h"
#include "mliScenery_valid.h"
#include "mli_frame_to_scenery.h"
#include "mliUserScenery.h"
#include "../frame/frame.h"

int mliScenery_from_io_tar(struct mliScenery *scenery, FILE *f)
{
        struct mli_Archive archive = mli_Archive_init();
        chk_dbg;
        chk_msg(mli_Archive_from_file(&archive, f),
                "Can't read archive from file.");
        chk_dbg;
        chk_msg(mliScenery_malloc_from_Archive(scenery, &archive),
                "Can't malloc Scenery from Archive.");
        chk_dbg;
        mli_Archive_free(&archive);
        return 1;
chk_error:
        return 0;
}

int mliScenery_malloc_from_path_tar(
        struct mliScenery *scenery,
        const char *path)
{
        FILE *f = fopen(path, "rb");
        chk_dbg;
        chk_msgf(f != NULL, ("Can't open path '%s'.", path))
                chk_msg(mliScenery_from_io_tar(scenery, f),
                        "Can't fread Scenery from file.");
        chk_dbg;
        fclose(f);
        return 1;
chk_error:
        return 0;
}

int mliScenery_malloc_from_Archive(
        struct mliScenery *scenery,
        const struct mli_Archive *archive)
{
        uint64_t num_robjects = 0u;
        uint64_t num_objects = 0u;
        uint64_t total_num_boundary_layers = 0u;

        struct mli_ResourceNameMap material_names = mli_ResourceNameMap_init();
        struct mli_Map object_names = mli_Map_init();
        struct mli_Frame root = mli_Frame_init();

        chk_dbg;
        chk_msg(mli_Materials_malloc_form_archive(
                        &scenery->materials, &material_names, archive),
                "Failed to malloc materials.");

        chk_dbg;
        chk(mli_Map_malloc(&object_names));

        chk_dbg;
        num_objects = mli_Archive_num_filename_prefix_sufix(
                archive, "geometry/objects/", ".obj");

        chk_dbg;
        chk_msg(mli_Geometry_malloc_objects(&scenery->geometry, num_objects),
                "Failed to malloc geometry.objects.");

        chk_dbg;
        chk_msg(mli_set_geometry_objects_and_names_from_archive(
                        &scenery->geometry, &object_names, archive),
                "Failed to malloc geometry.objects.");

        chk_dbg;
        chk_msg(mli_check_malloc_root_frame_from_Archive(
                        &root,
                        archive,
                        &object_names,
                        scenery->geometry.objects,
                        &material_names.boundary_layers),
                "Failed to malloc and populate tree of frames.");

        chk_msg(mli_Frame_estimate_num_robjects_and_total_num_boundary_layers(
                        &root, &num_robjects, &total_num_boundary_layers),
                "Can not estimate num_robjects from tree of frames.");

        chk_msg(mli_GeometryToMaterialMap_malloc(
                        &scenery->geomap,
                        num_robjects,
                        total_num_boundary_layers),
                "Failed to malloc geometry to materials map.");

        chk_dbg;
        chk_msg(mli_Geometry_malloc_references(
                        &scenery->geometry, num_robjects),
                "Failed to malloc geometry.references.");

        chk_msg(mli_Frame_set_robjects_and_material_map(
                        &root, &scenery->geometry, &scenery->geomap),
                "Can not set robjects.");

        mli_ResourceNameMap_free(&material_names);
        mli_Map_free(&object_names);
        mli_Frame_free(&root);

        chk_msg(mliAccelerator_malloc_from_Geometry(
                        &scenery->accelerator, &scenery->geometry),
                "Failed to malloc accelerator from geometry.");

        chk_msg(mliScenery_valid(scenery), "Expected scenery to be valid.");
        chk_msg(mli_Geometry_warn_objects(&scenery->geometry),
                "Failed to warn about objects.");

        chk_dbg;
        return 1;
chk_error:
        return 0;
}
