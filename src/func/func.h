/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_FUNC_H_
#define MLI_FUNC_H_

#include <stdint.h>
#include <stdio.h>

struct mli_Func {
        uint64_t num_points;
        double *x;
        double *y;
};

int mli_Func_equal(const struct mli_Func a, const struct mli_Func b);
int mli_Func_fold_numeric(
        const struct mli_Func *a,
        const struct mli_Func *b,
        double *fold);
int mli_Func_fold_numeric_default_closest(
        const struct mli_Func *a,
        const struct mli_Func *b,
        double *fold);
int mli_Func_evaluate(const struct mli_Func *f, const double xarg, double *out);
int mli_Func_in_range(const struct mli_Func *f, const double xarg);
double mli_Func_evaluate_with_default_when_out_of_range(
        const struct mli_Func *f,
        const double xarg,
        const double default_value);
double mli_Func_evaluate_with_default_closest(
        const struct mli_Func *f,
        const double xarg);
int mli_Func_x_is_strictly_increasing(const struct mli_Func *f);
int mli_Func_malloc(struct mli_Func *f, const uint64_t num_points);
void mli_Func_free(struct mli_Func *f);
struct mli_Func mli_Func_init(void);
int mli_Func_is_valid(const struct mli_Func *func);
#endif
