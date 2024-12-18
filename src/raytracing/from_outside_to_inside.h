/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_RAYTRACING_FROM_OUTSIDE_TO_INSIDE_H_
#define MLI_RAYTRACING_FROM_OUTSIDE_TO_INSIDE_H_

#include "../vec/vec.h"

int mli_raytracing_from_outside_to_inside(
        const struct mli_Vec ray_direction_local,
        const struct mli_Vec surface_normal_local);
#endif
