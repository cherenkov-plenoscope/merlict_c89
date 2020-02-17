/* Copyright 2019 Sebastian Achim Mueller                                     */

CASE("mliPhotonHistory, malloc, free") {
    struct mliPhotonHistory history = mliPhotonHistory_init(42u);
    struct mliPhotonInteraction action;
    CHECK(history.num_reserved == 42u);
    CHECK(history.num == 0u);
    CHECK(history.actions == NULL);

    CHECK(mliPhotonHistory_malloc(&history));
    CHECK(history.num == 0u);

    action.type = MLI_PHOTON_CREATION;
    action.position = mliVec_set(1, 2, 1);
    action.refraction_going_to = 1u;
    action.absorbtion_going_to = 2u;
    action.refraction_coming_from = 1u;
    action.absorbtion_coming_from = 2u;

    history.actions[0] = action;
    CHECK(history.actions[0].type == MLI_PHOTON_CREATION);
    CHECK(mliVec_equal_margin(
        history.actions[0].position,
        mliVec_set(1, 2, 1),
        1e-7));
    CHECK(history.actions[0].refraction_going_to == 1u);
    CHECK(history.actions[0].absorbtion_going_to == 2u);

    mliPhotonHistory_free(&history);
}

CASE("mliPhotonHistory, push back") {
    const uint64_t num_reserved = 42u;
    uint64_t i;
    struct mliPhotonHistory history = mliPhotonHistory_init(num_reserved);
    struct mliPhotonInteraction action;
    CHECK(mliPhotonHistory_malloc(&history));
    for (i = 0u; i < num_reserved; i++) {
        action.type = i;
        action.position = mliVec_set(1, 2, 1);
        action.distance_of_ray = 13.0;
        action.refraction_going_to = 1u;
        action.absorbtion_going_to = 2u;
        action.refraction_coming_from = 1u;
        action.absorbtion_coming_from = 2u;
        CHECK(mliPhotonHistory_push_back(&history, action));
        CHECK(history.num == i+1);
    }
    CHECK(!mliPhotonHistory_push_back(&history, action));
    mliPhotonHistory_free(&history);
}
