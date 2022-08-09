/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliMaterials_equal.h"

int mliMaterials_media_equal(
        const struct mliMaterials *a,
        const struct mliMaterials *b)
{
        uint32_t i = 0u;
        chk_msg(a->num_media == b->num_media, "Different number of media.");
        for (i = 0; i < a->num_media; i++) {
                chk_msg(mliMedium_equal(&a->media[i], &b->media[i]),
                        "Different Medium.");
                chk_msg(mliName_equal(&a->medium_names[i], &b->medium_names[i]),
                        "Different medium-name.");
        }
        return 1;
error:
        fprintf(stderr, "In materials.media[%u].\n", i);
        return 0;
}

int mliMaterials_surfaces_equal(
        const struct mliMaterials *a,
        const struct mliMaterials *b)
{
        uint32_t i = 0u;
        chk_msg(a->num_surfaces == b->num_surfaces,
                "Different number of surfaces.");
        for (i = 0; i < a->num_surfaces; i++) {
                chk_msg(mliSurface_equal(&a->surfaces[i], &b->surfaces[i]),
                        "Different Surface.");
                chk_msg(mliName_equal(
                                &a->surface_names[i], &b->surface_names[i]),
                        "Different surface-name.");
        }
        return 1;
error:
        fprintf(stderr, "In materials.surfaces[%u].\n", i);
        return 0;
}

int mliMaterials_boundary_layers_equal(
        const struct mliMaterials *a,
        const struct mliMaterials *b)
{
        uint32_t i = 0u;
        chk_msg(a->num_boundary_layers == b->num_boundary_layers,
                "Different number of boundary_layers.");
        for (i = 0; i < a->num_boundary_layers; i++) {
                chk_msg(mliBoundaryLayer_equal(
                                a->boundary_layers[i], b->boundary_layers[i]),
                        "Different boundary_layer.");
                chk_msg(mliName_equal(
                                &a->boundary_layer_names[i],
                                &b->boundary_layer_names[i]),
                        "Different boundary_layer-name.");
        }
        return 1;
error:
        fprintf(stderr, "In materials.boundary_layers[%u].\n", i);
        return 0;
}

int mliMaterials_equal(
        const struct mliMaterials *a,
        const struct mliMaterials *b)
{
        chk_msg(a->default_medium == b->default_medium,
                "Different default_medium.");
        chk_msg(mliMaterials_media_equal(a, b), "Different media.");
        chk_msg(mliMaterials_surfaces_equal(a, b), "Different surfaces.");
        chk_msg(mliMaterials_boundary_layers_equal(a, b),
                "Different boundary_layers.");
        return 1;
error:
        return 0;
}
