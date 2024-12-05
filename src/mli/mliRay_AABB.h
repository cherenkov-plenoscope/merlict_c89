/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIRAY_AABB_H_
#define MLIRAY_AABB_H_

#include "mliRay.h"
#include "mliAABB.h"

void mliRay_aabb_intersections(
        const struct mliRay ray,
        const struct mliAABB aabb,
        double *t_near,
        double *t_far);
int mliRay_aabb_intersections_is_valid_given_near_and_far(
        const double t_near,
        const double t_far);
int mliRay_has_overlap_aabb(const struct mliRay ray, const struct mliAABB aabb);

#endif
