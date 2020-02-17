/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLISPHERICALCAPHEX_H_
#define MERLICT_MLISPHERICALCAPHEX_H_

#include <math.h>
#include "mli_math.h"

struct mliSphericalCapHex {
    double curvature_radius;
    double inner_hex_radius;
};

int mliSphericalCapHex_is_equal(
    const struct mliSphericalCapHex a,
    const struct mliSphericalCapHex b)
{
    if (a.curvature_radius != b.curvature_radius) return 0;
    if (a.inner_hex_radius != b.inner_hex_radius) return 0;
    return 1;
}

double mliSphericalCapHex_bounding_radius(
    const struct mliSphericalCapHex cap)
{
    const double outer_hex_radius = cap.inner_hex_radius*MLI_2_OVER_SQRT3;
    return mli_hypot(
        outer_hex_radius,
        mli_z_sphere(
            cap.inner_hex_radius*MLI_2_OVER_SQRT3,
            cap.curvature_radius));
}

#endif
