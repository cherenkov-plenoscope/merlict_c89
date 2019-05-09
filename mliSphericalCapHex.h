/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLISPHERICALCAPHEX_H_
#define MERLICT_MLISPHERICALCAPHEX_H_

#include <math.h>
#include "mliMath.h"

typedef struct {
    float curvature_radius;
    float inner_hex_radius;
} mliSphericalCapHex;

int mliSphericalCapHex_is_equal(
    const mliSphericalCapHex a,
    const mliSphericalCapHex b) {
    if (a.curvature_radius != b.curvature_radius) return 0;
    if (a.inner_hex_radius != b.inner_hex_radius) return 0;
    return 1;}

float mliSphericalCapHex_bounding_radius(
    const mliSphericalCapHex cap) {
    const double outer_hex_radius = cap.inner_hex_radius*MLI_INNER_TO_OUTER_HEX;
    return hypot(
        outer_hex_radius,
        z_sphere(
            cap.inner_hex_radius*MLI_INNER_TO_OUTER_HEX,
            cap.curvature_radius));}

#endif
