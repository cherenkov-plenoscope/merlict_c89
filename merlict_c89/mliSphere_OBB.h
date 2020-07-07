/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISPHERE_OBB_H_
#define MERLICT_C89_MLISPHERE_OBB_H_

#include <math.h>

#include "mli_math.h"

int mliSphere_has_overlap_obb(
        const double radius,
        const struct mliVec translation,
        const struct mliOBB obb);
struct mliOBB mliSphere_obb(
        const double radius,
        const struct mliVec translation);
#endif
