/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_BOUNDARYLAYER_MEDIUM_TRANSPARENT_H_
#define MLI_BOUNDARYLAYER_MEDIUM_TRANSPARENT_H_

#define MLI_BOUNDARYLAYER_MEDIUM_TYPE_TRANSPARENT 100

struct mli_BoundaryLayerMedium_Transparent {
        uint64_t refraction_spectrum;
};

int mli_BoundaryLayer_Medium_Transparent_equal(
        const struct mli_BoundaryLayerMedium_Transparent *a,
        const struct mli_BoundaryLayerMedium_Transparent *b);

#endif
