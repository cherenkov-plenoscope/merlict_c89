/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "boundarylayer_surface_mirror.h"

int mli_BoundaryLayer_Surface_Mirror_equal(
        const struct mli_BoundaryLayer_Surface_Mirror *a,
        const struct mli_BoundaryLayer_Surface_Mirror *b)
{
        if (a->reflection_spectrum != b->reflection_spectrum) {
                return 0;
        }
        return 1;
}
