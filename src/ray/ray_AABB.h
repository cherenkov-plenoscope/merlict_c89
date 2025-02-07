/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_RAY_AABB_H_
#define MLI_RAY_AABB_H_

#include "../bool/bool.h"
#include "../ray/ray.h"
#include "../aabb/aabb.h"

void mli_Ray_aabb_intersections(
        const struct mli_Ray ray,
        const struct mli_AABB aabb,
        double *t_near,
        double *t_far);
mli_bool mli_Ray_aabb_intersections_is_valid_given_near_and_far(
        const double t_near,
        const double t_far);
mli_bool mli_Ray_has_overlap_aabb(
        const struct mli_Ray ray,
        const struct mli_AABB aabb);

#endif
