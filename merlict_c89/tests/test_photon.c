/* Copyright 2019 Sebastian Achim Mueller                                     */

CASE("simple propagation")
{
        struct mliPrng prng = mliPrng_init_MT19937(0u);
        struct mliScenery scenery = mliScenery_init();
        struct mliDynPhotonInteraction history = mliDynPhotonInteraction_init();
        struct mliIntersectionSurfaceNormal intersection;
        struct mliSide side_coming_from, side_going_to;
        uint64_t max_interactions = 16;

        uint64_t MED_VACUUM = 0;
        uint64_t MED_GLASS = 0;

        struct mliPhoton photon;
        photon.ray = mliRay_set(mliVec_set(0, 0, -3), mliVec_set(0, 0, 1));
        photon.wavelength = 600e-9;
        photon.id = 0;

        CHECK(mliScenery_malloc_from_tar(
                &scenery,
                "merlict_c89/"
                "tests/"
                "resources/"
                "sceneries/"
                "002.tar"));

        CHECK(mliName_find_idx(
                scenery.materials.medium_names,
                scenery.materials.num_media,
                "glass",
                &MED_GLASS));
        CHECK(mliName_find_idx(
                scenery.materials.medium_names,
                scenery.materials.num_media,
                "vacuum",
                &MED_VACUUM));

        CHECK(scenery.materials.default_medium == MED_VACUUM);

        CHECK(mli_query_intersection_with_surface_normal(
                &scenery, photon.ray, &intersection));

        CHECK(mliVec_equal_margin(
                intersection.position, mliVec_set(0, 0, 0), 1e-9));
        CHECK(mliVec_equal_margin(
                intersection.surface_normal, mliVec_set(0, 0, -1), 1e-9));
        CHECK_MARGIN(intersection.distance_of_ray, 3., 1e-9);

        CHECK(scenery.materials.num_media == 2);

        side_coming_from = _mli_side_coming_from(&scenery, &intersection);
        side_going_to = _mli_side_going_to(&scenery, &intersection);

        CHECK(side_going_to.medium == MED_GLASS);
        CHECK(scenery.materials.surfaces[side_going_to.surface].material ==
              MLI_MATERIAL_TRANSPARENT);

        CHECK(side_coming_from.medium == MED_VACUUM);
        CHECK(scenery.materials.surfaces[side_coming_from.surface].material ==
              MLI_MATERIAL_TRANSPARENT);

        CHECK(mliDynPhotonInteraction_malloc(&history, max_interactions));

        CHECK(mli_propagate_photon(
                &scenery, &history, &photon, &prng, max_interactions));

        CHECK(history.size >= 1);

        mliDynPhotonInteraction_print(&history, &scenery.geometry);

        mliScenery_free(&scenery);
        mliDynPhotonInteraction_free(&history);
}
