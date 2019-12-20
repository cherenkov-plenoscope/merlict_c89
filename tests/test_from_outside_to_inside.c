/* Copyright 2019-2020 Sebastian Achim Mueller                                */

/* from_outside_to_inside */
{
    mliVec normal = {0., 0., 1.};
    mliVec direction = {0., 0., -1.};
    CHECK(mli_ray_runs_from_outside_to_inside(direction, normal));
}

{
    mliVec normal = {0., 0., 1.};
    mliVec direction = {0., 0., 1.};
    CHECK(!mli_ray_runs_from_outside_to_inside(direction, normal));
}