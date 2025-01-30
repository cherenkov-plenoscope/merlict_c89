/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "boundarylayer_surface_lambertian.h"

int mli_BoundaryLayer_Surface_Lambertian_equal(
        const struct mli_BoundaryLayer_Surface_Lambertian *a,
        const struct mli_BoundaryLayer_Surface_Lambertian *b)
{
        if (a->reflection_spectrum != b->reflection_spectrum) {
                return 0;
        }
        return 1;
}
