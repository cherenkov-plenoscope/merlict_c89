/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "geometrytomaterialmap_serialize.h"
#include "../magicid/magicid.h"
#include "../chk/chk.h"

int mli_GeometryToMaterialMap_from_io(
        struct mli_GeometryToMaterialMap *geomap,
        struct mli_IO *f)
{
        uint32_t num_robjects = 0u;
        uint32_t total_num_boundary_layers = 0u;
        struct mli_MagicId magic;

        /* magic identifier */
        chk_IO_read(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk(mli_MagicId_has_word(&magic, "mli_GeometryToMaterialMap"));
        mli_MagicId_warn_version(&magic);

        /* payload */
        chk_IO_read(&num_robjects, sizeof(uint32_t), 1u, f);
        chk_IO_read(&total_num_boundary_layers, sizeof(uint32_t), 1u, f);
        chk_msg(mli_GeometryToMaterialMap_malloc(
                        geomap, num_robjects, total_num_boundary_layers),
                "Failed to malloc mli_GeometryToMaterialMap.");
        chk_IO_read(
                geomap->boundary_layers,
                sizeof(uint32_t),
                geomap->total_num_boundary_layers,
                f);
        chk_IO_read(
                geomap->first_boundary_layer_in_robject,
                sizeof(uint32_t),
                geomap->num_robjects,
                f);
        return 1;
chk_error:
        return 0;
}

int mli_GeometryToMaterialMap_to_io(
        const struct mli_GeometryToMaterialMap *geomap,
        struct mli_IO *f)
{
        struct mli_MagicId magic;

        /* magic identifier */
        chk(mli_MagicId_set(&magic, "mli_GeometryToMaterialMap"));
        chk_IO_write(&magic, sizeof(struct mli_MagicId), 1u, f);

        /* payload */
        chk_IO_write(&geomap->num_robjects, sizeof(uint32_t), 1, f);
        chk_IO_write(
                &geomap->total_num_boundary_layers, sizeof(uint32_t), 1, f);
        chk_IO_write(
                geomap->boundary_layers,
                sizeof(uint32_t),
                geomap->total_num_boundary_layers,
                f);
        chk_IO_write(
                geomap->first_boundary_layer_in_robject,
                sizeof(uint32_t),
                geomap->num_robjects,
                f);
        return 1;
chk_error:
        return 0;
}
