/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIRAY_H_
#define MLIRAY_H_

#include <stdio.h>
#include "mliVec.h"

struct mliRay {
        struct mliVec support;
        struct mliVec direction;
};

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
