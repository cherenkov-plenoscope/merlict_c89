/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_FROM_OUTSIDE_TO_INSIDE_H_
#define MERLICT_C89_MLI_FROM_OUTSIDE_TO_INSIDE_H_

#include "mliVec.h"

int mli_ray_runs_from_outside_to_inside(
        const struct mliVec ray_direction_local,
        const struct mliVec surface_normal_local);
#endif
