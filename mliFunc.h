/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIFUNC_H_
#define MERLICT_MLIFUNC_H_

#include <stdlib.h>
#include <math.h>
#include "mli_debug.h"
#include "mliMath.h"

typedef struct {
    uint32_t num_points;
    double *x;
    double *y;
} mliFunc;

mliFunc mliFunc_init() {
    mliFunc f;
    f.num_points = 0u;
    f.x = NULL;
    f.y = NULL;
    return f;}

void mliFunc_free(mliFunc *f) {
    free(f->x);
    free(f->y);
    *f = mliFunc_init();}

int mliFunc_malloc(mliFunc* f) {
    mli_malloc(f->x, double, f->num_points);
    mli_malloc(f->y, double, f->num_points);
    return 1;
error:
    mliFunc_free(f);
    return 0;}

int mliFunc_x_is_causal(mliFunc* f) {
    uint32_t i;
    for (i = 1; i < f->num_points; i++) {
        if (f->x[i] <= f->x[i - 1])
            return 0;
    }
    return 1;}

int mliFunc_evaluate(
    mliFunc* f,
    const double xarg,
    double *out) {
    uint32_t idx = mli_upper_compare_double(f->x, f->num_points, xarg);
    if (idx == 0) {
        return 1;
    } else if (idx == f->num_points) {
        return 1;
    } else {
        (*out) = f->y[idx];
        return 0;
    }
}

int mliFunc_fwrite(const mliFunc *func, FILE* f) {
    mli_fwrite(&func->num_points, sizeof(uint32_t), 1u, f);
    mli_fwrite(func->x, sizeof(double), func->num_points, f);
    mli_fwrite(func->y, sizeof(double), func->num_points, f);
    return 1;
error:
    return 0;}

int mliFunc_malloc_from_file(mliFunc *func, FILE* f) {
    mli_fread(&func->num_points, sizeof(uint32_t), 1u, f);
    mliFunc_malloc(func);
    mli_fread(func->x, sizeof(double), func->num_points, f);
    mli_fread(func->y, sizeof(double), func->num_points, f);
    return 1;
error:
    mliFunc_free(func);
    return 0;}

int mliFunc_is_equal(const mliFunc a, const mliFunc b) {
    uint64_t i;
    if (a.num_points != b.num_points ) return 0;
    for (i = 0; i < a.num_points; i++) {
        if (a.x[i] != b.x[i]) return 0;
        if (a.y[i] != b.y[i]) return 0;}
    return 1;}

#endif
