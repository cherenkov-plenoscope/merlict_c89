/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_BOUNDARYLAYER_SURFACE_PHONG_H_
#define MLI_BOUNDARYLAYER_SURFACE_PHONG_H_

#include "../io/io.h"

#define MLI_BOUNDARYLAYER_SURFACE_TYPE_PHONG 4000

struct mli_BoundaryLayer_Surface_Phong {
        uint64_t specular_reflection_spectrum;
        uint64_t diffuse_reflection_spectrum;
        double shininess_constant;
};

int mli_BoundaryLayer_Surface_Phong_equal(
        const struct mli_BoundaryLayer_Surface_Phong *a,
        const struct mli_BoundaryLayer_Surface_Phong *b);

int mli_BoundaryLayer_Surface_Phong_to_io(
        const struct mli_BoundaryLayer_Surface_Phong *self,
        struct mli_IO *f);
int mli_BoundaryLayer_Surface_Phong_from_io(
        struct mli_BoundaryLayer_Surface_Phong *self,
        struct mli_IO *f);

#endif
