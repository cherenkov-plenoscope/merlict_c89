/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISURFACE_H_
#define MERLICT_C89_MLISURFACE_H_

#include <stdint.h>

#define MLI_MATERIAL_PHONG 100u
#define MLI_MATERIAL_TRANSPARENT 102u

struct mliSurface {
        uint32_t material;

        uint32_t specular_reflection;
        uint32_t diffuse_reflection;

        /*
         *  The color is only relevant for fast rendering of images.
         *  Color will not effect the propagation of photons.
         */
        uint32_t color;
};

int mli_material_type_to_string(const uint32_t type, char *s);
int mli_material_type_from_string(const char *s, uint32_t *id);
int mliSurface_is_equal(const struct mliSurface a, const struct mliSurface b);
#endif
