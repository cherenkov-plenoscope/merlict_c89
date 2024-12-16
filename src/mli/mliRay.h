/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIRAY_H_
#define MLIRAY_H_

#include <stdio.h>
#include "../vec/vec.h"

struct mliRay {
        struct mli_Vec support;
        struct mli_Vec direction;
};

struct mli_Vec mliRay_at(const struct mliRay *ray, const double t);
struct mliRay mliRay_set(
        const struct mli_Vec support,
        const struct mli_Vec direction);
int mliRay_sphere_intersection(
        const struct mli_Vec support,
        const struct mli_Vec direction,
        const double radius,
        double *minus_solution,
        double *plus_solution);
#endif
