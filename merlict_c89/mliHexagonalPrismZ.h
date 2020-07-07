/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIHEXAGONALPRISMZ_H_
#define MERLICT_C89_MLIHEXAGONALPRISMZ_H_

#include <math.h>
#include <stdint.h>
#include <assert.h>

#include "mliVec.h"
#include "mli_math.h"

struct mliVec mli_hexagon_corner(const uint64_t corner);
int mli_inside_hexagonal_prism_z(
        const struct mliVec v,
        const double inner_radius);
#endif
