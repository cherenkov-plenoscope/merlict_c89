/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_BARYCENTRIC_H_
#define MERLICT_C89_MLI_BARYCENTRIC_H_

#include "mliVec.h"

struct mliBarycentrigWeights {
        double a;
        double b;
        double c;
};

struct mliBarycentrigWeights mli_barycentric_weights(
        const struct mliVec a,
        const struct mliVec b,
        const struct mliVec c,
        const struct mliVec t);
#endif
