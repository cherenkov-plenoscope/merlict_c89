/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIFUNC_H_
#define MLIFUNC_H_

#include <stdint.h>
#include <stdio.h>

struct mliFunc {
        uint32_t num_points;
        double *x;
        double *y;
};

int mliFunc_equal(const struct mliFunc a, const struct mliFunc b);
int mliFunc_fold_numeric(
        const struct mliFunc *a,
        const struct mliFunc *b,
        double *fold);
int mliFunc_evaluate(const struct mliFunc *f, const double xarg, double *out);
int mliFunc_x_is_strictly_increasing(const struct mliFunc *f);
int mliFunc_malloc(struct mliFunc *f, const uint32_t num_points);
void mliFunc_free(struct mliFunc *f);
struct mliFunc mliFunc_init(void);
int mliFunc_is_valid(const struct mliFunc *func);
#endif
