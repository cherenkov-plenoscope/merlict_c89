/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliMaterials_equal.h"


int _mliMaterials_media_equal(
        const struct mliMaterials *a,
        const struct mliMaterials *b)
{
        uint32_t i = 0u;
        chk(a->num_media == b->num_media);
        for (i = 0; i < a->num_media; i++) {
                chk(mliMedium_equal(&a->media[i], &b->media[i]));
                chk(mliName_equal(&a->medium_names[i], &b->medium_names[i]));
        }
        return 1;
error:
        fprintf(stderr, "In materials.media[%u].\n", i);
        return 0;
}

int _mliMaterials_surfaces_equal(
        const struct mliMaterials *a,
        const struct mliMaterials *b)
{
        uint32_t i = 0u;
        chk(a->num_surfaces == b->num_surfaces);
        for (i = 0; i < a->num_surfaces; i++) {
                chk(mliSurface_equal(&a->surfaces[i], &b->surfaces[i]));
                chk(mliName_equal(&a->surface_names[i], &b->surface_names[i]));
        }
        return 1;
error:
        fprintf(stderr, "In materials.surfaces[%u].\n", i);
        return 0;
}

int _mliMaterials_boundary_layers_equal(
        const struct mliMaterials *a,
        const struct mliMaterials *b)
{
        uint32_t i = 0u;
        chk(a->num_boundary_layers == b->num_boundary_layers);
        for (i = 0; i < a->num_boundary_layers; i++) {
                chk(mliBoundaryLayer_equal(
                        a->boundary_layers[i], b->boundary_layers[i]));
                chk(mliName_equal(
                        &a->boundary_layer_names[i],
                        &b->boundary_layer_names[i]));
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
        chk(a->default_medium == b->default_medium);
        chk(_mliMaterials_media_equal(a, b));
        chk(_mliMaterials_surfaces_equal(a, b));
        chk(_mliMaterials_boundary_layers_equal(a, b));
        return 1;
error:
        return 0;
}
