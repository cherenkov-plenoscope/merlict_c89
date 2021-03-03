/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIRAY_H_
#define MERLICT_C89_MLIRAY_H_

#include <stdio.h>
#include "mliVec.h"

struct mliRay {
        struct mliVec support;
        struct mliVec direction;
} mliRay;

struct mliVec mliRay_at(const struct mliRay *ray, const double t);
struct mliRay mliRay_set(
        const struct mliVec support,
        const struct mliVec direction);
int mliRay_sphere_intersection(
        const struct mliVec support,
        const struct mliVec direction,
        const double radius,
        double *minus_solution,
        double *plus_solution);
#endif
