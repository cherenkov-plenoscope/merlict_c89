/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_RAY_AABB_H_
#define MLI_RAY_AABB_H_

#include "../ray/ray.h"
#include "../aabb/aabb.h"

void mliRay_aabb_intersections(
        const struct mliRay ray,
        const struct mli_AABB aabb,
        double *t_near,
        double *t_far);
int mliRay_aabb_intersections_is_valid_given_near_and_far(
        const double t_near,
        const double t_far);
int mliRay_has_overlap_aabb(
        const struct mliRay ray,
        const struct mli_AABB aabb);

#endif
