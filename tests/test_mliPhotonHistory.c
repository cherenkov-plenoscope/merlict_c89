/* Copyright 2019 Sebastian Achim Mueller                                     */

CASE("mliPhotonHistory, malloc, free") {
    mliPhotonHistory history = mliPhotonHistory_init(42u);
    mliIntersection isec;
    CHECK(history.num_reserved == 42u);
    CHECK(history.num == 0u);
    CHECK(history.sections == NULL);
    CHECK(history.actions == NULL);

    CHECK(mliPhotonHistory_malloc(&history));
    CHECK(history.num == 0u);

    isec.object_idx = 37u;
    isec.position = mliVec_set(1, 2, 1);
    isec.surface_normal = mliVec_set(0, 0, 1);
    isec.distance_of_ray = 13.0;
    isec.from_outside_to_inside = 1;

    history.sections[0] = isec;
    history.actions[0] = 1337;

    CHECK(history.sections[0].object_idx == 37u);
    CHECK(mliVec_equal_margin(
            history.sections[0].position, mliVec_set(1, 2, 1), 1e-7));
    CHECK(mliVec_equal_margin(
            history.sections[0].surface_normal, mliVec_set(0, 0, 1), 1e-7));
    CHECK(history.sections[0].distance_of_ray == 13.0);
    CHECK(history.sections[0].from_outside_to_inside == 1);

    mliPhotonHistory_free(&history);
}
