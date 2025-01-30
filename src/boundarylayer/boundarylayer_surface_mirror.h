/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_BOUNDARYLAYER_SURFACE_MIRROR_H_
#define MLI_BOUNDARYLAYER_SURFACE_MIRROR_H_

#include "../io/io.h"

#define MLI_BOUNDARYLAYER_SURFACE_TYPE_MIRROR 3000

struct mli_BoundaryLayer_Surface_Mirror {
        uint64_t reflection_spectrum;
};

int mli_BoundaryLayer_Surface_Mirror_equal(
        const struct mli_BoundaryLayer_Surface_Mirror *a,
        const struct mli_BoundaryLayer_Surface_Mirror *b);

int mli_BoundaryLayer_Surface_Mirror_to_io(
        const struct mli_BoundaryLayer_Surface_Mirror *self,
        struct mli_IO *f);
int mli_BoundaryLayer_Surface_Mirror_from_io(
        struct mli_BoundaryLayer_Surface_Mirror *self,
        struct mli_IO *f);

#endif
