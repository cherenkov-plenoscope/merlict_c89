/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_SURFACE_COOK_TORRANCE_H_
#define MLI_SURFACE_COOK_TORRANCE_H_

#include "../io/io.h"

#define mli_Surface_TYPE_COOK_TORRANCE 5000

struct mli_Surface_Cook_Torrance {
        uint64_t specular_reflection_spectrum;
        uint64_t diffuse_reflection_spectrum;
        double surface_roughness_constant;
};

int mli_Surface_Cook_Torrance_equal(
        const struct mli_Surface_Cook_Torrance *a,
        const struct mli_Surface_Cook_Torrance *b);

int mli_Surface_Cook_Torrance_to_io(
        const struct mli_Surface_Cook_Torrance *self,
        struct mli_IO *f);
int mli_Surface_Cook_Torrance_from_io(
        struct mli_Surface_Cook_Torrance *self,
        struct mli_IO *f);

#endif
