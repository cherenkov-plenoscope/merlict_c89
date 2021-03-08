/* Copyright 2019 Sebastian Achim Mueller                                     */

CASE("simple propagation")
{
        struct mliMT19937 prng = mliMT19937_init(0u);
        struct mliScenery scenery = mliScenery_init();
        struct mliDynPhotonInteraction history = mliDynPhotonInteraction_init();
        struct mliIntersectionSurfaceNormal intersection;
        struct mliSide side_coming_from, side_going_to;
        struct mliSurface surf_coming_from, surf_going_to;
        struct mliMedium medi_coming_from, medi_going_to;
        uint64_t max_interactions = 16;

        uint64_t FNC_POSITIV_INFINITY = 0;
        uint64_t FNC_REFRACTION_GLASS = 0;
        uint64_t FNC_UNITY = 0;

        struct mliPhoton photon;
        photon.ray = mliRay_set(mliVec_set(0, 0, -3), mliVec_set(0, 0, 1));
        photon.wavelength = 600e-9;
        photon.id = 0;

        CHECK(mliScenery_malloc_from_tar(
                &scenery,
                "tests/"
                "resources/"
                "sceneries/"
                "002.tar"));

        CHECK(mliName_find_idx(
                scenery.materials.function_names,
                scenery.materials.num_functions,
                "refraction_glass",
                &FNC_REFRACTION_GLASS));
        CHECK(mliName_find_idx(
                scenery.materials.function_names,
                scenery.materials.num_functions,
                "unity",
                &FNC_UNITY));
        CHECK(mliName_find_idx(
                scenery.materials.function_names,
                scenery.materials.num_functions,
                "positiv_infinity",
                &FNC_POSITIV_INFINITY));

        CHECK(scenery.materials.default_medium == 0u);

        CHECK(mli_query_intersection_with_surface_normal(
                &scenery, photon.ray, &intersection));

        CHECK(mliVec_equal_margin(
                intersection.position, mliVec_set(0, 0, 0), 1e-9));
        CHECK(mliVec_equal_margin(
                intersection.surface_normal, mliVec_set(0, 0, -1), 1e-9));
        CHECK_MARGIN(intersection.distance_of_ray, 3., 1e-9);

        CHECK(scenery.materials.num_media == 2);
        CHECK(scenery.materials.num_functions == 4);

        side_coming_from = _mli_side_coming_from(&scenery, &intersection);
        surf_coming_from = scenery.materials.surfaces[side_coming_from.surface];
        medi_coming_from = scenery.materials.media[side_coming_from.medium];

        side_going_to = _mli_side_going_to(&scenery, &intersection);
        surf_going_to = scenery.materials.surfaces[side_going_to.surface];
        medi_going_to = scenery.materials.media[side_going_to.medium];

        CHECK(surf_going_to.material == MLI_MATERIAL_TRANSPARENT);
        CHECK(medi_going_to.refraction == FNC_REFRACTION_GLASS);
        CHECK(medi_going_to.absorbtion == FNC_POSITIV_INFINITY);

        CHECK(surf_coming_from.material == MLI_MATERIAL_TRANSPARENT);
        CHECK(medi_coming_from.refraction == FNC_UNITY);
        CHECK(medi_coming_from.absorbtion == FNC_POSITIV_INFINITY);

        CHECK(mliDynPhotonInteraction_malloc(&history, max_interactions));

        CHECK(mli_propagate_photon(
                &scenery, &history, &photon, &prng, max_interactions));

        CHECK(history.dyn.size >= 1);

        mliDynPhotonInteraction_print(&history, &scenery.geometry);

        mliScenery_free(&scenery);
        mliDynPhotonInteraction_free(&history);
}
