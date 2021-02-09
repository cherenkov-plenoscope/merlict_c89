/* Copyright 2019 Sebastian Achim Mueller                                     */

CASE("simple propagation")
{
        struct mliMT19937 prng = mliMT19937_init(0u);
        struct mliCombine combine = mliCombine_init();
        struct mliDynPhotonInteraction history = mliDynPhotonInteraction_init();
        struct mliIntersectionSurfaceNormal intersection;
        struct mliSide side_coming_from, side_going_to;
        struct mliSurface surf_coming_from, surf_going_to;
        struct mliMedium medi_coming_from, medi_going_to;
        uint64_t max_interactions = 16;

        uint64_t FNC_POSITIV_INFINITY = 0;
        uint64_t FNC_REFRACTION_GLASS = 1;
        uint64_t FNC_UNITY = 2;

        struct mliPhoton photon;
        photon.ray = mliRay_set(mliVec_set(0, 0, -3), mliVec_set(0, 0, 1));
        photon.wavelength = 600e-9;
        photon.id = 0;

        CHECK(mliCombine_malloc_from_tar(
                &combine,
                "tests/"
                "resources/"
                "sceneries/"
                "002.tar"));
        CHECK(combine.materials.default_medium == 0u);

        CHECK(mli_query_intersection_with_surface_normal(
                &combine, photon.ray, &intersection));

        CHECK(mliVec_equal_margin(
                intersection.position, mliVec_set(0, 0, 0), 1e-9));
        CHECK(mliVec_equal_margin(
                intersection.surface_normal, mliVec_set(0, 0, -1), 1e-9));
        CHECK_MARGIN(intersection.distance_of_ray, 3., 1e-9);

        CHECK(combine.materials.num_media == 2);
        CHECK(combine.materials.num_functions == 4);

        side_coming_from = _mli_side_coming_from(&combine.geometry, &intersection);
        surf_coming_from = combine.materials.surfaces[side_coming_from.surface];
        medi_coming_from = combine.materials.media[side_coming_from.medium];

        side_going_to = _mli_side_going_to(&combine.geometry, &intersection);
        surf_going_to = combine.materials.surfaces[side_going_to.surface];
        medi_going_to = combine.materials.media[side_going_to.medium];

        CHECK(surf_going_to.material == MLI_MATERIAL_TRANSPARENT);
        CHECK(medi_going_to.refraction == FNC_REFRACTION_GLASS);
        CHECK(medi_going_to.absorbtion == FNC_POSITIV_INFINITY);

        CHECK(surf_coming_from.material == MLI_MATERIAL_TRANSPARENT);
        CHECK(medi_coming_from.refraction == FNC_UNITY);
        CHECK(medi_coming_from.absorbtion == FNC_POSITIV_INFINITY);

        CHECK(mliDynPhotonInteraction_malloc(&history, max_interactions));

        CHECK(mli_propagate_photon(
                &combine, &history, &photon, &prng, max_interactions));

        CHECK(history.dyn.size >= 1);

        mliDynPhotonInteraction_print(&history, &combine.geometry);

        mliCombine_free(&combine);
        mliDynPhotonInteraction_free(&history);
}

/*
CASE("Do not leak out of closed box")
{
        struct mliMT19937 prng = mliMT19937_init(0u);
        struct mliDynPhoton photons = mliDynPhoton_init();
        struct mliDynPhotonInteraction history = mliDynPhotonInteraction_init();
        struct mliGeometry scenery = mliGeometry_init();
        struct mliOcTree octree = mliOcTree_init();
        const float wavelength = 433e-9;
        const float opening_angle = mli_deg2rad(180.0);
        const uint64_t num_photons = 100u;
        const uint64_t max_interactions = 100u;
        const int64_t sensor_id = 3;
        const struct mliVec center_of_crystal = mliVec_set(5e-3, 5e-3, 5e-3);
        uint64_t i;

        CHECK(mliGeometry_malloc_from_json_path(
                &scenery, "tests/resources/lyso_crystal_scenery.json"));

        CHECK(mliOcTree_malloc_from_scenery(&octree, &scenery));

        CHECK(mliDynPhoton_malloc(&photons, 0u));
        CHECK(photons.dyn.size == 0);
        CHECK(point_like_towards_z_opening_angle_num_photons(
                &photons, wavelength, opening_angle, num_photons, &prng));
        CHECK(photons.dyn.size == num_photons);
        for (i = 0; i < photons.dyn.size; i++) {
                photons.arr[i].ray.support = center_of_crystal;
        }

        for (i = 0; i < photons.dyn.size; i++) {
                struct mliPhotonInteraction final_interaction;
                struct mliPhoton photon = photons.arr[i];

                CHECK(mliDynPhotonInteraction_malloc(
                        &history, max_interactions));

                CHECK(mli_propagate_photon(
                        &scenery,
                        &octree,
                        &history,
                        &photon,
                        &prng,
                        max_interactions));

                mliDynPhotonInteraction_print(&history, &scenery);

                final_interaction = history.arr[history.dyn.size - 1];
                if (final_interaction.object_idx >= 0) {
                        int64_t id_primitive =
                                scenery.user_ids[final_interaction.object_idx];
                        if (id_primitive == sensor_id) {
                                struct mliVec point;
                                point = final_interaction.position;

                                CHECK(point.x >= 0.0);
                                CHECK(point.x <= 1e-2);

                                CHECK(point.y >= 0.0);
                                CHECK(point.y <= 1e-2);

                                CHECK(point.z >= 0.0);
                                CHECK(point.z <= 1e-2);
                        }
                }

                mliDynPhotonInteraction_free(&history);
        }

        mliOcTree_free(&octree);
        mliGeometry_free(&scenery);
        mliDynPhoton_free(&photons);
}
*/
