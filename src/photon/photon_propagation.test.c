/* Copyright 2019 Sebastian Achim Mueller                                     */

CASE("simple propagation")
{
        struct mli_Prng prng = mli_Prng_init_MT19937(0u);
        struct mli_Scenery scenery = mli_Scenery_init();
        struct mli_PhotonInteractionVector history =
                mli_PhotonInteractionVector_init();
        struct mli_IntersectionSurfaceNormal intersection;
        struct mli_BoundaryLayer_Side side_coming_from, side_going_to;
        uint64_t max_interactions = 16;

        uint64_t MED_VACUUM = 0;
        uint64_t MED_GLASS = 0;

        struct mli_Photon photon;
        photon.ray = mli_Ray_set(mli_Vec_init(0, 0, -3), mli_Vec_init(0, 0, 1));
        photon.wavelength = 600e-9;
        photon.id = 0;

        CHECK(mli_Scenery__from_path_cstr(
                &scenery,
                "data/"
                "mli/"
                "tests/"
                "resources/"
                "sceneries/"
                "002.tar"));

        CHECK(mli_String__find_idx_with_cstr(
                scenery.materials.medium_names,
                scenery.materials.num_media,
                "glass",
                &MED_GLASS));
        CHECK(mli_String__find_idx_with_cstr(
                scenery.materials.medium_names,
                scenery.materials.num_media,
                "vacuum",
                &MED_VACUUM));

        CHECK(scenery.materials.default_medium == MED_VACUUM);

        CHECK(mli_raytracing_query_intersection_with_surface_normal(
                &scenery, photon.ray, &intersection));

        CHECK(mli_Vec_equal_margin(
                intersection.position, mli_Vec_init(0, 0, 0), 1e-9));
        CHECK(mli_Vec_equal_margin(
                intersection.surface_normal, mli_Vec_init(0, 0, -1), 1e-9));
        CHECK_MARGIN(intersection.distance_of_ray, 3., 1e-9);

        CHECK(scenery.materials.num_media == 2);

        side_coming_from =
                mli_raytracing_get_side_coming_from(&scenery, &intersection);
        side_going_to =
                mli_raytracing_get_side_going_to(&scenery, &intersection);

        CHECK(side_going_to.medium == MED_GLASS);
        CHECK(scenery.materials.surfaces[side_going_to.surface].material ==
              MLI_SURFACE_TRANSPARENT);

        CHECK(side_coming_from.medium == MED_VACUUM);
        CHECK(scenery.materials.surfaces[side_coming_from.surface].material ==
              MLI_SURFACE_TRANSPARENT);

        CHECK(mli_PhotonInteractionVector_malloc(&history, max_interactions));

        CHECK(mli_propagate_photon(
                &scenery, &history, &photon, &prng, max_interactions));

        CHECK(history.size >= 1);

        mli_PhotonInteractionVector_print(&history, &scenery);

        mli_Scenery_free(&scenery);
        mli_PhotonInteractionVector_free(&history);
}
