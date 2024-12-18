/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("from_outside_to_inside, forward")
{
        struct mli_Vec normal = {0., 0., 1.};
        struct mli_Vec direction = {0., 0., -1.};
        CHECK(mli_trace_ray_runs_from_outside_to_inside(direction, normal));
}

CASE("from_outside_to_inside, backward")
{
        struct mli_Vec normal = {0., 0., 1.};
        struct mli_Vec direction = {0., 0., 1.};
        CHECK(!mli_trace_ray_runs_from_outside_to_inside(direction, normal));
}
