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
    struct mliMT19937 prng;
    struct mliScenery scenery = mliScenery_init();
    struct mliOcTree octree;
    mliPhotonHistory history = mliPhotonHistory_init(16u);
    struct mliIntersection intersection;
    struct mliSurface surf_coming_from, surf_going_to;
    mliPhoton photon = mliPhoton_set(
        mliRay_set(
            mliVec_set(0, 0, -3),
            mliVec_set(0, 0, 1)),
        600e-9);
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
        intersection.position, mliVec_set(0, 0, 0), 1e-9));
    CHECK(mliVec_equal_margin(
        intersection.surface_normal, mliVec_set(0, 0, 1), 1e-9));
    CHECK_MARGIN(intersection.distance_of_ray, 3., 1e-9);

    surf_coming_from = _mli_surface_coming_from(&scenery, &intersection);
    surf_going_to = _mli_surface_going_to(&scenery, &intersection);

    CHECK(surf_going_to.material == MLI_MATERIAL_TRANSPARENT);
    CHECK(surf_going_to.medium_refraction == 0);
    CHECK(surf_going_to.medium_absorbtion == 2);
    CHECK(surf_going_to.color == 0);

    CHECK(surf_coming_from.material == MLI_MATERIAL_TRANSPARENT);
    CHECK(surf_coming_from.medium_refraction == 1);
    CHECK(surf_coming_from.medium_absorbtion == 2);
    CHECK(surf_coming_from.color == 1);

    mliMT19937_init(&prng, 0u);
    CHECK(mliPhotonHistory_malloc(&history));
    history.actions[0].type = MLI_PHOTON_CREATION;
    history.actions[0].position = photon.ray.support;
    history.actions[0].refraction_going_to = 0u;
    history.actions[0].absorbtion_going_to = 2u;
    history.actions[0].refraction_coming_from = 0u;
    history.actions[0].absorbtion_coming_from = 2u;
    history.actions[0]._object_idx = -1;
    history.actions[0]._from_outside_to_inside = 1;
    history.num += 1;

    CHECK(mli_propagate_photon(
            &scenery,
            &octree,
            &history,
            &photon,
            &prng));

    mliPhotonHistory_print(&history);

    mliScenery_free(&scenery);
    mliOcTree_free(&octree);
    mliPhotonHistory_free(&history);
}
