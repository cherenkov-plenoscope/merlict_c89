/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_MATERIALS_EQUAL_H_
#define MLI_MATERIALS_EQUAL_H_

#include "../bool/bool.h"
#include "../materials/materials.h"

mli_bool mli_Materials_equal(
        const struct mli_Materials *a,
        const struct mli_Materials *b);
mli_bool mli_Materials_spectra_equal(
        const struct mli_Materials *a,
        const struct mli_Materials *b);
mli_bool mli_Materials_surfaces_equal(
        const struct mli_Materials *a,
        const struct mli_Materials *b);
mli_bool mli_Materials_media_equal(
        const struct mli_Materials *a,
        const struct mli_Materials *b);
mli_bool mli_Materials_boundary_layers_equal(
        const struct mli_Materials *a,
        const struct mli_Materials *b);
mli_bool mli_Materials_default_medium_equal(
        const struct mli_Materials *a,
        const struct mli_Materials *b);
#endif
