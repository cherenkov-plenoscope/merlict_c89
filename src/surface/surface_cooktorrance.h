/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_SURFACE_COOKTORRANCE_H_
#define MLI_SURFACE_COOKTORRANCE_H_

#include <stdint.h>
#include "../io/io.h"
struct mli_Map;
struct mli_String;

#define MLI_SURFACE_TYPE_COOKTORRANCE 5000

struct mli_Surface_CookTorrance {
        uint64_t reflection_spectrum;
        double diffuse_weight;
        double specular_weight;
        double roughness;
};

int mli_Surface_CookTorrance_equal(
        const struct mli_Surface_CookTorrance *a,
        const struct mli_Surface_CookTorrance *b);

int mli_Surface_CookTorrance_to_io(
        const struct mli_Surface_CookTorrance *self,
        struct mli_IO *f);
int mli_Surface_CookTorrance_from_io(
        struct mli_Surface_CookTorrance *self,
        struct mli_IO *f);

int mli_Surface_CookTorrance_from_json_string(
        struct mli_Surface_CookTorrance *self,
        const struct mli_Map *spectra_names,
        const struct mli_String *json_string);

#endif
