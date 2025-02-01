/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_SURFACE_PHONG_H_
#define MLI_SURFACE_PHONG_H_

#include "../io/io.h"
struct mli_Map;
struct mli_String;

#define mli_Surface_TYPE_PHONG 4000

struct mli_Surface_Phong {
        uint64_t reflection_spectrum;
        double diffuse_weight;
        double specular_weight;
        double shininess;
};

int mli_Surface_Phong_equal(
        const struct mli_Surface_Phong *a,
        const struct mli_Surface_Phong *b);

int mli_Surface_Phong_to_io(
        const struct mli_Surface_Phong *self,
        struct mli_IO *f);
int mli_Surface_Phong_from_io(struct mli_Surface_Phong *self, struct mli_IO *f);

int mli_Surface_Phong_from_json_string(
        struct mli_Surface_Phong *self,
        const struct mli_Map *spectra_names,
        const struct mli_String *json_string);

#endif
