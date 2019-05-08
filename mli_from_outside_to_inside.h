/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLI_FROM_OUTSIDE_TO_INSIDE_H_
#define MERLICT_MLI_FROM_OUTSIDE_TO_INSIDE_H_

#include "mliVec.h"

int mli_ray_runs_from_outside_to_inside(
    const mliVec ray_direction_local,
    const mliVec surface_normal_local) {
    const double proj = mliVec_dot(surface_normal_local, ray_direction_local);
    if (proj < 0.)
        return 1;
    else
        return 0;}

#endif
