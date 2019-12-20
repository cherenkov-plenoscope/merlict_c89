/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLISURFACE_H_
#define MERLICT_MLISURFACE_H_

#include <stdint.h>

#define MLI_MATERIAL_PHONG 100u
#define MLI_MATERIAL_METAL 101u
#define MLI_MATERIAL_TRANSPARENT 102u

typedef struct {
    uint32_t material;

    uint32_t medium_refraction;
    uint32_t medium_absorbtion;
    uint32_t boundary_layer_specular_reflection;
    uint32_t boundary_layer_diffuse_reflection;
    uint32_t boundary_layer_transmission;

    /* The color is only relevant for fast rendering of images.
     * Color will not effect the propagation of photons. */
    uint32_t color;
} mliSurface;

int mliSurface_is_equal(const mliSurface a, const mliSurface b) {
    if (a.material != b.material) return 0;
    if (a.medium_refraction != b.medium_refraction) return 0;
    if (a.medium_absorbtion != b.medium_absorbtion) return 0;
    if (a.boundary_layer_specular_reflection !=
        b.boundary_layer_specular_reflection) return 0;
    if (a.boundary_layer_diffuse_reflection !=
        b.boundary_layer_diffuse_reflection) return 0;
    if (a.boundary_layer_transmission !=
        b.boundary_layer_transmission) return 0;
    if (a.color != b.color) return 0;
    return 1;}

#endif
