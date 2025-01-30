/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_BOUNDARYLAYER_MEDIUM_TRANSPARENT_H_
#define MLI_BOUNDARYLAYER_MEDIUM_TRANSPARENT_H_

#include "../io/io.h"

#define MLI_BOUNDARYLAYER_MEDIUM_TYPE_TRANSPARENT 100

struct mli_BoundaryLayerMedium_Transparent {
        uint64_t refraction_spectrum;
};

int mli_BoundaryLayer_Medium_Transparent_equal(
        const struct mli_BoundaryLayerMedium_Transparent *a,
        const struct mli_BoundaryLayerMedium_Transparent *b);

int mli_BoundaryLayer_Medium_Transparent_to_io(
        const struct mli_BoundaryLayerMedium_Transparent *self,
        struct mli_IO *f);
int mli_BoundaryLayer_Medium_Transparent_from_io(
        struct mli_BoundaryLayerMedium_Transparent *self,
        struct mli_IO *f);

#endif
