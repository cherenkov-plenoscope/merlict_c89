// Copyright 2019 Sebastian Achim Mueller
#ifndef MERLICT_MLIFUNC_H_
#define MERLICT_MLIFUNC_H_

#include <math.h>


struct mliFunc {
    uint32_t num_points;
    float *x;
    float *y;};

void mliFunc_init(
    struct mliFunc* f,
    const uint32_t num_points) {
    f->num_points = num_points;
    f->x = (float*)malloc(f->num_points*sizeof(float));
    f->y = (float*)malloc(f->num_points*sizeof(float));}

void mliFunc_free(struct mliFunc *f) {
    free(f->x);
    free(f->y);
    f->num_points = 0;}

bool mliFunc_x_is_causal(struct mliFunc* f) {
    for (uint32_t i = 1; i < f->num_points; i++) {
        if (f->x[i] <= f->x[i - 1])
            return false;}
    return true;}

uint32_t mliFunc_upper_compare(
    struct mliFunc* f,
    const float xarg,
    bool v=false) {
    uint32_t first, last, middle;
    first = 0;
    last = f->num_points - 1;
    middle = (last - first)/2;
    if (f->num_points == 0)
        return 1;
    while (first < last) {
        if (f->x[middle] > xarg) {
            if (v) printf("f->x[%d] > xarg\n",middle);
            last = middle;
        } else if(f->x[middle] <= xarg) {
            if (v) printf("f->x[%d] <= xarg\n",middle);
            first = middle + 1;
        }
        middle = (last - first)/2;
    }
    if (v) printf("first: %d\n", first);
    if (first + 1 == f->num_points) {
        return f->num_points;}
    return first;}

int mliFunc_evaluate(
    struct mliFunc* f,
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

#endif  // MERLICT_MLIFUNC_H_
