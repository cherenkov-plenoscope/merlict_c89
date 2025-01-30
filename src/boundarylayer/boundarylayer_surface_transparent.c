/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "boundarylayer_surface_transparent.h"

int mli_BoundaryLayer_Surface_Transparent_equal(
        const struct mli_BoundaryLayer_Surface_Transparent *a,
        const struct mli_BoundaryLayer_Surface_Transparent *b)
{
        if (a->nothing != b->nothing) {
                return 0;
        }
        return 1;
}
