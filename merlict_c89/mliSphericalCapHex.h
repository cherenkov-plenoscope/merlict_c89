/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISPHERICALCAPHEX_H_
#define MERLICT_C89_MLISPHERICALCAPHEX_H_

#include <math.h>

#include "mli_math.h"

struct mliSphericalCapHex {
        double curvature_radius;
        double inner_hex_radius;
};

double mliSphericalCapHex_bounding_radius(const struct mliSphericalCapHex cap);
int mliSphericalCapHex_is_equal(
        const struct mliSphericalCapHex a,
        const struct mliSphericalCapHex b);
#endif
