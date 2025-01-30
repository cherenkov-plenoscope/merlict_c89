/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "boundarylayer_medium_transparent.h"

int mli_BoundaryLayer_Medium_Transparent_equal(
        const struct mli_BoundaryLayerMedium_Transparent *a,
        const struct mli_BoundaryLayerMedium_Transparent *b)
{
        if (a->refraction_spectrum != b->refraction_spectrum) {
                return 0;
        }
        return 1;
}
