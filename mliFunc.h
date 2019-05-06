/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIFUNC_H_
#define MERLICT_MLIFUNC_H_

#include <stdlib.h>
#include <math.h>
#include "mliVector.h"


typedef struct {
    uint32_t num_points;
    float *x;
    float *y;
} mliFunc;

void mliFunc_malloc(
    mliFunc* f,
    const uint32_t num_points) {
    f->num_points = num_points;
    f->x = (float*)malloc(f->num_points*sizeof(float));
    f->y = (float*)malloc(f->num_points*sizeof(float));}

void mliFunc_free(mliFunc *f) {
    free(f->x);
    free(f->y);
    f->num_points = 0;}

int mliFunc_x_is_causal(mliFunc* f) {
    uint32_t i;
    for (i = 1; i < f->num_points; i++) {
        if (f->x[i] <= f->x[i - 1])
            return 0;
    }
    return 1;}

uint32_t mliFunc_upper_compare(
    mliFunc* f,
    const float xarg) {
    uint32_t first, last, middle;
    first = 0;
    last = f->num_points - 1;
    middle = (last - first)/2;
    if (f->num_points == 0)
        return 1;
    while (first < last) {
        if (f->x[middle] > xarg) {
            last = middle;
        } else if(f->x[middle] <= xarg) {
            first = middle + 1;
        }
        middle = (last - first)/2;
    }
    if (first + 1 == f->num_points) {
        return f->num_points;}
    return first;}

int mliFunc_evaluate(
    mliFunc* f,
    const float xarg,
    float *out) {
    uint32_t idx = mliFunc_upper_compare(f, xarg);
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
    fwrite(&func->num_points, sizeof(uint32_t), 1u, f);
    fwrite(func->x, sizeof(float), func->num_points, f);
    fwrite(func->y, sizeof(float), func->num_points, f);
    return EXIT_SUCCESS;}

int mliFunc_fread(mliFunc *func, FILE* f) {
    fread(&func->num_points, sizeof(uint32_t), 1u, f);
    mliFunc_malloc(func, func->num_points);
    fread(func->x, sizeof(float), func->num_points, f);
    fread(func->y, sizeof(float), func->num_points, f);
    return EXIT_SUCCESS;}

int mliFunc_is_equal(const mliFunc a, const mliFunc b) {
    int i;
    if (a.num_points != b.num_points ) return 0;
    for (i = 0; i < a.num_points; i++) {
        if (a.x[i] != b.x[i]) return 0;
        if (a.y[i] != b.y[i]) return 0;}
    return 1;}

#endif
