/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "boundarylayer_surface_phong.h"

int mli_BoundaryLayer_Surface_Phong_equal(
        const struct mli_BoundaryLayer_Surface_Phong *a,
        const struct mli_BoundaryLayer_Surface_Phong *b)
{
        if (a->specular_reflection_spectrum !=
            b->specular_reflection_spectrum) {
                return 0;
        }
        if (a->diffuse_reflection_spectrum != b->diffuse_reflection_spectrum) {
                return 0;
        }
        if (a->shininess_constant != b->shininess_constant) {
                return 0;
        }
        return 1;
}
