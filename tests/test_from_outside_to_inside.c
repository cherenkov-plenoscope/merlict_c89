/* Copyright 2019-2020 Sebastian Achim Mueller                                */

CASE("from_outside_to_inside, forward") {
    mliVec normal = {0., 0., 1.};
    mliVec direction = {0., 0., -1.};
    CHECK(mli_ray_runs_from_outside_to_inside(direction, normal));
}

CASE("from_outside_to_inside, backward") {
    mliVec normal = {0., 0., 1.};
    mliVec direction = {0., 0., 1.};
    CHECK(!mli_ray_runs_from_outside_to_inside(direction, normal));
}