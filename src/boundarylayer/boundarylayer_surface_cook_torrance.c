/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "boundarylayer_surface_cook_torrance.h"

int mli_BoundaryLayer_Surface_Cook_Torrance_equal(
        const struct mli_BoundaryLayer_Surface_Cook_Torrance *a,
        const struct mli_BoundaryLayer_Surface_Cook_Torrance *b)
{
        if (a->specular_reflection_spectrum !=
            b->specular_reflection_spectrum) {
                return 0;
        }
        if (a->diffuse_reflection_spectrum != b->diffuse_reflection_spectrum) {
                return 0;
        }
        if (a->surface_roughness_constant != b->surface_roughness_constant) {
                return 0;
        }
        return 1;
}
