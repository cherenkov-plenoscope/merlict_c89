/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "materials_equal.h"
#include "../chk/chk.h"

int mli_Materials_media_equal(
        const struct mli_Materials *a,
        const struct mli_Materials *b)
{
        uint64_t i = 0u;
        chk_msg(a->media.size == b->media.size, "Different number of media.");
        for (i = 0; i < a->media.size; i++) {
                chk_msg(mli_Medium_equal(
                                &a->media.array[i], &b->media.array[i]),
                        "Medium is different.");
        }
        return 1;
chk_error:
        fprintf(stderr, "In materials.media[%lu].\n", i);
        return 0;
}

int mli_Materials_surfaces_equal(
        const struct mli_Materials *a,
        const struct mli_Materials *b)
{
        uint64_t i = 0u;
        chk_msg(a->surfaces.size == b->surfaces.size,
                "Different number of surfaces.");
        for (i = 0; i < a->surfaces.size; i++) {
                chk_msg(mli_Surface_equal(
                                &a->surfaces.array[i], &b->surfaces.array[i]),
                        "Surface is different.");
        }
        return 1;
chk_error:
        fprintf(stderr, "In materials.surfaces[%lu].\n", i);
        return 0;
}

int mli_Materials_boundary_layers_equal(
        const struct mli_Materials *a,
        const struct mli_Materials *b)
{
        uint64_t i = 0u;
        chk_msg(a->boundary_layers.size == b->boundary_layers.size,
                "Different number of boundary_layers.");
        for (i = 0; i < a->boundary_layers.size; i++) {
                chk_msg(mli_BoundaryLayer_equal(
                                &a->boundary_layers.array[i],
                                &b->boundary_layers.array[i]),
                        "Boundary layer is different.");
        }
        return 1;
chk_error:
        fprintf(stderr, "In materials.boundary_layers[%lu].\n", i);
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
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

int mli_Materials_default_medium_equal(
        const struct mli_Materials *a,
        const struct mli_Materials *b)
{
        chk_msg(a->default_medium == b->default_medium,
                "Different default_medium.");
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

int mli_Materials_equal(
        const struct mli_Materials *a,
        const struct mli_Materials *b)
{
        chk(mli_Materials_spectra_equal(a, b));
        chk(mli_Materials_media_equal(a, b));
        chk(mli_Materials_surfaces_equal(a, b));
        chk(mli_Materials_boundary_layers_equal(a, b));
        chk(mli_Materials_default_medium_equal(a, b));
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}
