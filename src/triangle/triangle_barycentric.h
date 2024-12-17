/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_TRIANGLE_BARYCENTRIC_H_
#define MLI_TRIANGLE_BARYCENTRIC_H_

#include "../vec/vec.h"

struct mli_triangle_BarycentrigWeights {
        double a;
        double b;
        double c;
};

struct mli_triangle_BarycentrigWeights mli_triangle_barycentric_weights(
        const struct mli_Vec a,
        const struct mli_Vec b,
        const struct mli_Vec c,
        const struct mli_Vec t);
#endif
