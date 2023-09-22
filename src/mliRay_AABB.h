/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIRAY_AABB_H_
#define MLIRAY_AABB_H_

#include "mliRay.h"
#include "mliAABB.h"

int mliRay_has_overlap_aabb(
        const struct mliRay ray,
        const struct mliAABB aabb,
        double *ray_parameter);
#endif
