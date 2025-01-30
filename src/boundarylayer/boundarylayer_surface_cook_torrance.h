/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_BOUNDARYLAYER_SURFACE_COOK_TORRANCE_H_
#define MLI_BOUNDARYLAYER_SURFACE_COOK_TORRANCE_H_

#define MLI_BOUNDARYLAYER_SURFACE_TYPE_COOK_TORRANCE 5000

struct mli_BoundaryLayer_Surface_Cook_Torrance {
        uint64_t specular_reflection_spectrum;
        uint64_t diffuse_reflection_spectrum;
        double surface_roughness_constant;
};

int mli_BoundaryLayer_Surface_Cook_Torrance_equal(
        const struct mli_BoundaryLayer_Surface_Cook_Torrance *a,
        const struct mli_BoundaryLayer_Surface_Cook_Torrance *b);

#endif
