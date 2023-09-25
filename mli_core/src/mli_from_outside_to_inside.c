/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_from_outside_to_inside.h"

int mli_ray_runs_from_outside_to_inside(
        const struct mliVec ray_direction_local,
        const struct mliVec surface_normal_local)
{
        const double proj =
                mliVec_dot(surface_normal_local, ray_direction_local);
        if (proj < 0.)
                return 1;
        else
                return 0;
}
