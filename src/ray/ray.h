/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_RAY_H_
#define MLI_RAY_H_

#include <stdio.h>
#include "../chk/chk.h"
#include "../vec/vec.h"

struct mli_Ray {
        struct mli_Vec support;
        struct mli_Vec direction;
};

struct mli_Vec mli_Ray_at(const struct mli_Ray *ray, const double t);
struct mli_Ray mli_Ray_set(
        const struct mli_Vec support,
        const struct mli_Vec direction);
chk_rc mli_Ray_sphere_intersection(
        const struct mli_Vec support,
        const struct mli_Vec direction,
        const double radius,
        double *minus_solution,
        double *plus_solution);
#endif
