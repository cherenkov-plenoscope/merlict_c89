/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_SURFACE_LAMBERTIAN_H_
#define MLI_SURFACE_LAMBERTIAN_H_

#include "../io/io.h"

#define mli_Surface_TYPE_LAMBERTIAN 2000

struct mli_Surface_Lambertian {
        uint64_t reflection_spectrum;
};

int mli_Surface_Lambertian_equal(
        const struct mli_Surface_Lambertian *a,
        const struct mli_Surface_Lambertian *b);

int mli_Surface_Lambertian_to_io(
        const struct mli_Surface_Lambertian *self,
        struct mli_IO *f);
int mli_Surface_Lambertian_from_io(
        struct mli_Surface_Lambertian *self,
        struct mli_IO *f);

#endif
