/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_BOUNDARYLAYER_SURFACE_MIRROR_H_
#define MLI_BOUNDARYLAYER_SURFACE_MIRROR_H_

#define MLI_BOUNDARYLAYER_SURFACE_TYPE_MIRROR 3000

struct mli_BoundaryLayer_Surface_Mirror {
        uint64_t reflection_spectrum;
};

int mli_BoundaryLayer_Surface_Mirror_equal(
        const struct mli_BoundaryLayer_Surface_Mirror *a,
        const struct mli_BoundaryLayer_Surface_Mirror *b);

#endif
