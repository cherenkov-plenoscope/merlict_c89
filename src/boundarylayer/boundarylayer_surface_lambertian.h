/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_BOUNDARYLAYER_SURFACE_LAMBERTIAN_H_
#define MLI_BOUNDARYLAYER_SURFACE_LAMBERTIAN_H_

#define MLI_BOUNDARYLAYER_SURFACE_TYPE_LAMBERTIAN 2000

struct mli_BoundaryLayer_Surface_Lambertian {
        uint64_t reflection_spectrum;
};

int mli_BoundaryLayer_Surface_Lambertian_equal(
        const struct mli_BoundaryLayer_Surface_Lambertian *a,
        const struct mli_BoundaryLayer_Surface_Lambertian *b);

#endif
