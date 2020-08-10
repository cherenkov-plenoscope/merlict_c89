/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIFUNC_H_
#define MERLICT_C89_MLIFUNC_H_

#include <stdlib.h>
#include <math.h>

#include "mli_debug.h"
#include "mli_math.h"

struct mliFunc {
        uint32_t num_points;
        double *x;
        double *y;
};

int mliFunc_cpy(struct mliFunc *destination, const struct mliFunc *source);
int mliFunc_is_equal(const struct mliFunc a, const struct mliFunc b);
int mliFunc_malloc_from_file(struct mliFunc *func, FILE *f);
int mliFunc_fwrite(const struct mliFunc *func, FILE *f);
int mliFunc_fold_numeric(
        const struct mliFunc *a,
        const struct mliFunc *b,
        double *fold);
int mliFunc_evaluate(const struct mliFunc *f, const double xarg, double *out);
double mli_linear_interpolate(
        const double xarg,
        const double x0,
        const double y0,
        const double x1,
        const double y1);
int mliFunc_x_is_causal(struct mliFunc *f);
int mliFunc_malloc(struct mliFunc *f, const uint32_t num_points);
void mliFunc_free(struct mliFunc *f);
struct mliFunc mliFunc_init(void);
#endif