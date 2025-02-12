/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "scenery_tar.h"
#include <math.h>
#include <stdint.h>
#include "../chk/chk.h"
#include "scenery_valid.h"
#include "../geometry/geometry_set_from_frame.h"
#include "../geometry/geometry_from_archive.h"
#include "../materials/materials_from_archive.h"
#include "../frame/frame.h"
#include "../frame/frame_from_archive.h"

chk_rc mli_Scenery_from_io_tar(struct mli_Scenery *self, struct mli_IO *f)
{
        struct mli_Archive archive = mli_Archive_init();
        chk_dbg;
        chk_msg(mli_Archive_from_io(&archive, f),
                "Can't read archive from file.");
        chk_dbg;
        chk_msg(mli_Scenery_malloc_from_Archive(self, &archive),
                "Can't malloc Scenery from Archive.");
        chk_dbg;
        mli_Archive_free(&archive);
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Scenery__from_path_cstr(struct mli_Scenery *self, const char *path)
{
        struct mli_IO f = mli_IO_init();
        chk_msgf(
                mli_IO_open_file_cstr(&f, path, "r"),
                ("Can't open path '%s'.", path));
        chk_msgf(
                mli_Scenery_from_io_tar(self, &f),
                ("Can't fread Scenery from path '%s'.", path));
        mli_IO_close(&f);
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Scenery_malloc_from_Archive(
        struct mli_Scenery *self,
        const struct mli_Archive *archive)
{
        uint64_t num_robjects = 0u;
        uint64_t num_objects = 0u;
        uint64_t total_num_boundary_layers = 0u;

        struct mli_materials_Names material_names = mli_materials_Names_init();
        struct mli_Map object_names = mli_Map_init();
        struct mli_Frame root = mli_Frame_init();

        chk_dbg;
        chk_msg(mli_Materials_from_Archive(
                        &self->materials, &material_names, archive),
                "Failed to malloc materials.");

        chk_dbg;
        chk(mli_Map_malloc(&object_names));

        chk_dbg;
        num_objects = mli_Archive_num_filename_prefix_sufix(
                archive, "geometry/objects/", ".obj");

        chk_dbg;
        chk_msg(mli_Geometry_malloc_objects(&self->geometry, num_objects),
                "Failed to malloc geometry.objects.");

        chk_dbg;
        chk_msg(mli_Geometry_from_archive(
                        &self->geometry, &object_names, archive),
                "Failed to malloc geometry.objects.");

        chk_dbg;
        chk_msg(mli_Frame_from_Archive(
                        &root,
                        archive,
                        &object_names,
                        self->geometry.objects,
                        &material_names.boundary_layers),
                "Failed to malloc and populate tree of frames.");

        chk_msg(mli_Frame_estimate_num_robjects_and_total_num_boundary_layers(
                        &root, &num_robjects, &total_num_boundary_layers),
                "Can not estimate num_robjects from tree of frames.");

        chk_msg(mli_GeometryToMaterialMap_malloc(
                        &self->geomap, num_robjects, total_num_boundary_layers),
                "Failed to malloc geometry to materials map.");

        chk_dbg;
        chk_msg(mli_Geometry_malloc_references(&self->geometry, num_robjects),
                "Failed to malloc geometry.references.");

        chk_msg(mli_Geometry_set_robjects_and_material_map_from_frame(
                        &root, &self->geometry, &self->geomap),
                "Can not set robjects.");

        mli_materials_Names_free(&material_names);
        mli_Map_free(&object_names);
        mli_Frame_free(&root);

        chk_msg(mli_Accelerator_malloc_from_Geometry(
                        &self->accelerator, &self->geometry),
                "Failed to malloc accelerator from geometry.");

        chk_msg(mli_Scenery_valid(self), "Expected scenery to be valid.");
        chk_msg(mli_Geometry_warn_objects(&self->geometry),
                "Failed to warn about objects.");

        chk_dbg;
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}
