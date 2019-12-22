/* Copyright 2019 Sebastian Achim Mueller                                     */

CASE("mliPhotonHistory, malloc, free") {
    mliPhotonHistory history = mliPhotonHistory_init();
    mliIntersection isec;
    CHECK(history.num_reserved == 0u);
    CHECK(history.num == 0u);
    CHECK(history.sections == NULL);
    CHECK(history.actions == NULL);

    history.num_reserved = 42u;
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

CASE("mliPhoton_set") {
    mliPhoton ph = mliPhoton_set(
        mliRay_set(mliVec_set(1, 2, 3), mliVec_set(0, 0, 1)),
        350e-9);
    CHECK(ph.wavelength == 350e-9);
    CHECK(mliVec_equal_margin(ph.ray.support, mliVec_set(1, 2, 3), 1e-9));
    CHECK(mliVec_equal_margin(ph.ray.direction, mliVec_set(0, 0, 1), 1e-9));
}

CASE("simple propagation") {
    mliScenery scenery = mliScenery_init();
    mliOcTree octree;
    mliIntersection intersection;
    mliSurfaces surfaces;
    mliPhoton photon = mliPhoton_set(
        mliRay_set(mliVec_set(0, 0, -3), mliVec_set(0, 0, 1)), 600e-9);
    CHECK(mliScenery_malloc_from_json_path(
        &scenery,
        "tests/resources/glass_cylinder_in_air.json"));
    octree = mliOcTree_from_scenery(&scenery);

    mli_ray_octree_traversal(
        &scenery,
        &octree,
        photon.ray,
        &intersection);

    CHECK(mliVec_equal_margin(
        intersection.position, mliVec_set(0, 0, -1), 1e-9));
    CHECK(mliVec_equal_margin(
        intersection.surface_normal, mliVec_set(0, 0, 1), 1e-9));
    CHECK_MARGIN(intersection.distance_of_ray, 2., 1e-9);

    surfaces = mliScenery_object_surfaces(&scenery, intersection.object_idx);
    CHECK(surfaces.inner == 0);
    CHECK(surfaces.outer == 1);

    mliScenery_free(&scenery);
    mliOcTree_free(&octree);
}
