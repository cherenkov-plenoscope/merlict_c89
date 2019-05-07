/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLISPHERICALCAPHEX_H_
#define MERLICT_MLISPHERICALCAPHEX_H_

#include <math.h>

#define MLI_INNER_TO_OUTER_HEX 1.1547005383792517

typedef struct {
    float curvature_radius;
    float inner_hex_radius;
} mliSphericalCapHeagonal;

int mliSphericalCapHeagonal_is_equal(
    const mliSphericalCapHeagonal a,
    const mliSphericalCapHeagonal b) {
    if (a.curvature_radius != b.curvature_radius) return 0;
    if (a.inner_hex_radius != b.inner_hex_radius) return 0;
    return 1;}

float mliSphericalCapHex_bounding_radius(
    const mliSphericalCapHeagonal cap) {
    return hypot(
        cap.inner_hex_radius*MLI_INNER_TO_OUTER_HEX,
        z_sphere(
            cap.inner_hex_radius*MLI_INNER_TO_OUTER_HEX,
            cap.curvature_radius));}

#endif
