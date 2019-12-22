/* Copyright 2019 Sebastian Achim Mueller                                     */

CASE("mliPhoton_set") {
    mliPhoton ph = mliPhoton_set(
        mliRay_set(mliVec_set(1, 2, 3), mliVec_set(0, 0, 1)),
        350e-9);
    CHECK(ph.wavelength == 350e-9);
    CHECK(mliVec_equal_margin(ph.ray.support, mliVec_set(1, 2, 3), 1e-9));
    CHECK(mliVec_equal_margin(ph.ray.direction, mliVec_set(0, 0, 1), 1e-9));
}

CASE("simple propagation") {
    mliMT19937 prng;
    mliScenery scenery = mliScenery_init();
    mliOcTree octree;
    mliPhotonHistory history = mliPhotonHistory_init(16u);
    mliIntersection intersection;
    mliSurface surf_coming_from, surf_going_to;
    mliEnv env;
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

    env.scenery = &scenery;
    env.octree = &octree;
    env.history = &history;
    env.photon = &photon;
    env.prng = &prng;

    surf_coming_from = _mli_surface_coming_from(&env, &intersection);
    surf_going_to = _mli_surface_going_to(&env, &intersection);

    CHECK(surf_coming_from.material == 100);
    CHECK(surf_coming_from.medium_refraction == 0);
    CHECK(surf_coming_from.color == 0);

    CHECK(surf_going_to.material == 100);
    CHECK(surf_going_to.medium_refraction == 1);
    CHECK(surf_going_to.color == 1);

    mliMT19937_init(&prng, 0u);
    /* CHECK(mli_propagate_photon(&env)); */

    mliScenery_free(&scenery);
    mliOcTree_free(&octree);
}
