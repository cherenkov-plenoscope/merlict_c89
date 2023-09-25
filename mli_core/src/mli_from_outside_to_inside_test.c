/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "../../mli_testing/include/mli_testing.h"
#include "mli_from_outside_to_inside.h"

CASE("from_outside_to_inside, forward")
{
        struct mliVec normal = {0., 0., 1.};
        struct mliVec direction = {0., 0., -1.};
        CHECK(mli_ray_runs_from_outside_to_inside(direction, normal));
}

CASE("from_outside_to_inside, backward")
{
        struct mliVec normal = {0., 0., 1.};
        struct mliVec direction = {0., 0., 1.};
        CHECK(!mli_ray_runs_from_outside_to_inside(direction, normal));
}
