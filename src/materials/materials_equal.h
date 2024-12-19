/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_MATERIALS_EQUAL_H_
#define MLI_MATERIALS_EQUAL_H_

#include "../materials/materials.h"

int mli_Materials_equal(
        const struct mli_Materials *a,
        const struct mli_Materials *b);
int mli_Materials_spectra_equal(
        const struct mli_Materials *a,
        const struct mli_Materials *b);
int mli_Materials_surfaces_equal(
        const struct mli_Materials *a,
        const struct mli_Materials *b);
int mli_Materials_media_equal(
        const struct mli_Materials *a,
        const struct mli_Materials *b);
int mli_Materials_boundary_layers_equal(
        const struct mli_Materials *a,
        const struct mli_Materials *b);
#endif
