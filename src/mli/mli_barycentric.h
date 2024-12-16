/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_BARYCENTRIC_H_
#define MLI_BARYCENTRIC_H_

#include "../vec/vec.h"

struct mliBarycentrigWeights {
        double a;
        double b;
        double c;
};

struct mliBarycentrigWeights mli_barycentric_weights(
        const struct mli_Vec a,
        const struct mli_Vec b,
        const struct mli_Vec c,
        const struct mli_Vec t);
#endif
