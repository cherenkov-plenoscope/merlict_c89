/* Copyright 2019 Sebastian Achim Mueller                                     */

CASE("simple propagation")
{
        struct mli_Prng prng = mli_Prng_init_MT19937(0u);
        struct mli_Scenery scenery = mli_Scenery_init();
        struct mli_PhotonInteractionVector history =
                mli_PhotonInteractionVector_init();
        struct mli_IntersectionSurfaceNormal intersection;
        struct mli_IntersectionLayer ilayer = mli_IntersectionLayer_init();
        uint64_t max_interactions = 16;

        uint64_t MED_VACUUM = 0;
        uint64_t MED_GLASS = 1;

        struct mli_Photon photon;
        photon.ray = mli_Ray_set(mli_Vec_init(0, 0, -3), mli_Vec_init(0, 0, 1));
        photon.wavelength = 600e-9;
        photon.id = 0;

        CHECK(mli_Scenery__from_path_cstr(
                &scenery,
                "data/"
                "sceneries/"
                "002.tar"));

        CHECK(mli_String_equal_cstr(
                &scenery.materials.media.array[MED_GLASS].name, "glass"));
        CHECK(mli_String_equal_cstr(
                &scenery.materials.media.array[MED_VACUUM].name, "vacuum"));
        CHECK(scenery.materials.default_medium == MED_VACUUM);

        CHECK(mli_raytracing_query_intersection_with_surface_normal(
                &scenery, photon.ray, &intersection));

        CHECK(mli_Vec_equal_margin(
                intersection.position, mli_Vec_init(0, 0, 0), 1e-9));
        CHECK(mli_Vec_equal_margin(
                intersection.surface_normal, mli_Vec_init(0, 0, -1), 1e-9));
        CHECK_MARGIN(intersection.distance_of_ray, 3., 1e-9);

        CHECK(scenery.materials.media.size == 2);

        ilayer = mli_raytracing_get_intersection_layer(&scenery, &intersection);

        CHECK(ilayer.side_going_to.medium ==
              &scenery.materials.media.array[MED_GLASS]);
        CHECK(ilayer.side_going_to.surface->type ==
              MLI_SURFACE_TYPE_TRANSPARENT);

        CHECK(ilayer.side_coming_from.medium ==
              &scenery.materials.media.array[MED_VACUUM]);
        CHECK(ilayer.side_coming_from.surface->type ==
              MLI_SURFACE_TYPE_TRANSPARENT);

        CHECK(mli_PhotonInteractionVector_malloc(&history, max_interactions));

        CHECK(mli_propagate_photon(
                &scenery, &history, &photon, &prng, max_interactions));

        CHECK(history.size >= 1);

        mli_PhotonInteractionVector_print(&history, &scenery);

        mli_Scenery_free(&scenery);
        mli_PhotonInteractionVector_free(&history);
}
