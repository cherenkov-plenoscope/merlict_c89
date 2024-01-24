/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliGeometryToMaterialMap_serialize.h"
#include "mliMagicId.h"
#include "../../chk_debug/src/chk_debug.h"

int mliGeometryToMaterialMap_malloc_fread(
        struct mliGeometryToMaterialMap *geomap,
        FILE *f)
{
        uint32_t num_robjects = 0u;
        uint32_t total_num_boundary_layers = 0u;
        struct mliMagicId magic;

        /* magic identifier */
        chk_fread(&magic, sizeof(struct mliMagicId), 1u, f);
        chk(mliMagicId_has_word(&magic, "mliGeometryToMaterialMap"));
        mliMagicId_warn_version(&magic);

        /* payload */
        chk_fread(&num_robjects, sizeof(uint32_t), 1u, f);
        chk_fread(&total_num_boundary_layers, sizeof(uint32_t), 1u, f);
        chk_msg(mliGeometryToMaterialMap_malloc(
                        geomap, num_robjects, total_num_boundary_layers),
                "Failed to malloc mliGeometryToMaterialMap.");
        chk_fread(
                geomap->boundary_layers,
                sizeof(uint32_t),
                geomap->total_num_boundary_layers,
                f);
        chk_fread(
                geomap->first_boundary_layer_in_robject,
                sizeof(uint32_t),
                geomap->num_robjects,
                f);
        return 1;
error:
        return 0;
}

int mliGeometryToMaterialMap_fwrite(
        const struct mliGeometryToMaterialMap *geomap,
        FILE *f)
{
        struct mliMagicId magic;

        /* magic identifier */
        chk(mliMagicId_set(&magic, "mliGeometryToMaterialMap"));
        chk_fwrite(&magic, sizeof(struct mliMagicId), 1u, f);

        /* payload */
        chk_fwrite(&geomap->num_robjects, sizeof(uint32_t), 1, f);
        chk_fwrite(&geomap->total_num_boundary_layers, sizeof(uint32_t), 1, f);
        chk_fwrite(
                geomap->boundary_layers,
                sizeof(uint32_t),
                geomap->total_num_boundary_layers,
                f);
        chk_fwrite(
                geomap->first_boundary_layer_in_robject,
                sizeof(uint32_t),
                geomap->num_robjects,
                f);
        return 1;
error:
        return 0;
}
