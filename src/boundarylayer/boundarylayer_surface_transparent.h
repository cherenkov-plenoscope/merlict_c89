/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_BOUNDARYLAYER_SURFACE_TRANSPARENT_H_
#define MLI_BOUNDARYLAYER_SURFACE_TRANSPARENT_H_

#define MLI_BOUNDARYLAYER_SURFACE_TYPE_TRANSPARENT 1000

struct mli_BoundaryLayer_Surface_Transparent {
        uint64_t nothing;
};

int mli_BoundaryLayer_Surface_Transparent_equal(
        const struct mli_BoundaryLayer_Surface_Transparent *a,
        const struct mli_BoundaryLayer_Surface_Transparent *b);

#endif
