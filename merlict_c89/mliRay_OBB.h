/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIRAY_OBB_H_
#define MERLICT_C89_MLIRAY_OBB_H_

#include "mliRay.h"
#include "mliOBB.h"

int mliRay_has_overlap_obb(
        const struct mliRay ray,
        const struct mliOBB obb,
        double *ray_parameter);
#endif
