/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "materials_equal.h"
#include "../chk/chk.h"

int mli_Materials_media_equal(
        const struct mli_Materials *a,
        const struct mli_Materials *b)
{
        uint32_t i = 0u;
        chk_msg(a->num_media == b->num_media, "Different number of media.");
        for (i = 0; i < a->num_media; i++) {
                chk_msg(mli_Medium_equal(&a->media[i], &b->media[i]),
                        "Different Medium.");
                chk_msg(mli_String_equal(
                                &a->medium_names[i], &b->medium_names[i]),
                        "Different medium-name.");
        }
        return 1;
chk_error:
        fprintf(stderr, "In materials.media[%u].\n", i);
        return 0;
}

int mli_Materials_surfaces_equal(
        const struct mli_Materials *a,
        const struct mli_Materials *b)
{
        uint32_t i = 0u;
        chk_msg(a->num_surfaces == b->num_surfaces,
                "Different number of surfaces.");
        for (i = 0; i < a->num_surfaces; i++) {
                chk_msg(mli_Surface_equal(&a->surfaces[i], &b->surfaces[i]),
                        "Different Surface.");
                chk_msg(mli_String_equal(
                                &a->surface_names[i], &b->surface_names[i]),
                        "Different surface-name.");
        }
        return 1;
chk_error:
        fprintf(stderr, "In materials.surfaces[%u].\n", i);
        return 0;
}

int mli_Materials_boundary_layers_equal(
        const struct mli_Materials *a,
        const struct mli_Materials *b)
{
        uint32_t i = 0u;
        chk_msg(a->num_boundary_layers == b->num_boundary_layers,
                "Different number of boundary_layers.");
        for (i = 0; i < a->num_boundary_layers; i++) {
                chk_msg(mli_BoundaryLayer_equal(
                                a->boundary_layers[i], b->boundary_layers[i]),
                        "Different boundary_layer.");
                chk_msg(mli_String_equal(
                                &a->boundary_layer_names[i],
                                &b->boundary_layer_names[i]),
                        "Different boundary_layer-name.");
        }
        return 1;
chk_error:
        fprintf(stderr, "In materials.boundary_layers[%u].\n", i);
        return 0;
}

int mli_Materials_layers2_equal(
        const struct mli_Materials *a,
        const struct mli_Materials *b)
{
        uint32_t i = 0u;
        chk_msg(a->layers2.size == b->layers2.size,
                "Different number of boundary layers.");
        for (i = 0; i < a->layers2.size; i++) {
                chk_msg(mli_BoundaryLayer2_equal_physics(
                                &a->layers2.array[i], &b->layers2.array[i]),
                        "Different boundary layer.");
                chk_msg(mli_String_equal(
                                &a->layers2.array[i].name,
                                &b->layers2.array[i].name),
                        "Different boundary layer name.");
        }
        return 1;
chk_error:
        fprintf(stderr, "In materials.layers2[%u].\n", i);
        return 0;
}

int mli_Materials_spectra_equal(
        const struct mli_Materials *a,
        const struct mli_Materials *b)
{
        uint64_t i = 0u;
        chk_msg(a->spectra.size == b->spectra.size,
                "Different number of spectra.");
        for (i = 0; i < a->spectra.size; i++) {
                chk_msg(mli_Spectrum_equal(
                                &a->spectra.array[i], &b->spectra.array[i]),
                        "Spectrum is not equal.");
        }
        return 1;
chk_error:
        return 0;
}

int mli_Materials_equal(
        const struct mli_Materials *a,
        const struct mli_Materials *b)
{
        chk_msg(mli_Materials_spectra_equal(a, b), "Different spectra.");
        chk_msg(mli_Materials_default_medium_equal(a, b),
                "Different default medium.");

        chk_msg(mli_Materials_media_equal(a, b), "Different media.");
        chk_msg(mli_Materials_surfaces_equal(a, b), "Different surfaces.");
        chk_msg(mli_Materials_boundary_layers_equal(a, b),
                "Different boundary_layers.");
        /*
        chk_msg(mli_Materials_layers2_equal(a, b),
                "Different layers2.");
        */
        return 1;
chk_error:
        return 0;
}

int mli_Materials_default_medium_equal(
        const struct mli_Materials *a,
        const struct mli_Materials *b)
{
        chk_msg(a->default_medium == b->default_medium,
                "Different default_medium.");
        return 1;
chk_error:
        return 0;
}
