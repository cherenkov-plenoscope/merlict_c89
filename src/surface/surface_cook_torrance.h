/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_SURFACE_COOK_TORRANCE_H_
#define MLI_SURFACE_COOK_TORRANCE_H_

#include "../io/io.h"

#define MLI_SURFACE_TYPE_COOK_TORRANCE 5000

struct mli_Surface_Cook_Torrance {
        uint64_t reflection_spectrum;
        double diffuse_weight;
        double specular_weight;
        double roughness;
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

int mli_Surface_Cook_Torrance_from_json_string(
        struct mli_Surface_Cook_Torrance *self,
        const struct mli_Map *spectra_names,
        const struct mli_String *json_string);

#endif
