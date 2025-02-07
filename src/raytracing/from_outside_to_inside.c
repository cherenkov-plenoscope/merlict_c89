/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "from_outside_to_inside.h"

mli_bool mli_raytracing_from_outside_to_inside(
        const struct mli_Vec ray_direction_local,
        const struct mli_Vec surface_normal_local)
{
        const double proj =
                mli_Vec_dot(surface_normal_local, ray_direction_local);
        if (proj < 0.)
                return MLI_TRUE;
        else
                return MLI_FALSE;
}
