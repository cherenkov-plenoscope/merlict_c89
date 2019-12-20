/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIFUNC_H_
#define MERLICT_MLIFUNC_H_

#include <stdlib.h>
#include <math.h>
#include "mli_debug.h"
#include "mli_math.h"

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
        if (f->x[i] <= f->x[i - 1]) {
            return 0;}}
    return 1;}

double mli_linear_interpolate(
    const double xarg,
    const double x0,
    const double y0,
    const double x1,
    const double y1) {
    /*
        |
    y1 -|            o
        |
    y0 -|    o
        |       xarg
        +----|---|---|----
            x0       x1

    f(x) = m*x + b
    m = (y1 - y0)/(x1 - x0)
    y0 = m*x0 + b
    b = y0 - m*x0
    */
    const double m = (y1 - y0)/(x1 - x0);
    const double b = y0 - m*x0;
    return m*xarg + b;}

int mliFunc_evaluate(mliFunc* f, const double xarg, double *out) {
    double y1, y0, x1, x0;
    uint32_t idx = mli_upper_compare_double(f->x, f->num_points, xarg);
    if (idx == 0) {
        mli_sentinel("mliFunc argument below lower bound.");
    } else if (idx == f->num_points) {
        mli_sentinel("mliFunc argument larger upper bound.");
    } else {
        y1 = f->y[idx];
        y0 = f->y[idx - 1u];
        x1 = f->x[idx];
        x0 = f->x[idx - 1u];
        (*out) = mli_linear_interpolate(xarg, x0, y0, x1, y1);
    }
    return 1;
error:
    return 0;}

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
    mli_check(
        mliFunc_x_is_causal(func),
        "Expected function x-arguments to be ascending, but they are not.");
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

int mliFunc_cpy(mliFunc* destination, const mliFunc* source) {
    uint64_t p;
    mli_check(
        destination->num_points == source->num_points,
        "Expected source and destination mliFunc to have same num_points.");
    for (p = 0; p < destination->num_points; p++) {
        destination->x[p] = source->x[p];
        destination->y[p] = source->y[p];}
    return 1;
error:
    return 0;}

#endif
