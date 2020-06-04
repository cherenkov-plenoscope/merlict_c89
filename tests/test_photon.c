/* Copyright 2019 Sebastian Achim Mueller                                     */

CASE("simple propagation")
{
        struct mliMT19937 prng = mliMT19937_init(0u);
        struct mliScenery scenery = mliScenery_init();
        struct mliOcTree octree = mliOcTree_init();
        struct mliDynPhotonInteraction history = mliDynPhotonInteraction_init();
        struct mliIntersection intersection;
        struct mliSide side_coming_from, side_going_to;
        struct mliSurface surf_coming_from, surf_going_to;
        struct mliMedium medi_coming_from, medi_going_to;
        uint64_t max_interactions = 16;

        struct mliPhoton photon;
        photon.ray = mliRay_set(mliVec_set(0, 0, -3), mliVec_set(0, 0, 1));
        photon.wavelength = 600e-9;
        photon.simulation_truth_id = 0;

        CHECK(mliScenery_malloc_from_json_path(
                &scenery,
                "tests/resources/glass_cylinder_in_air.json"));
        CHECK(scenery.default_medium == 0u);
        CHECK(mliOcTree_malloc_from_scenery(&octree, &scenery));

        mli_ray_octree_traversal(&scenery, &octree, photon.ray, &intersection);

        CHECK(mliVec_equal_margin(
                intersection.position, mliVec_set(0, 0, 0), 1e-9));
        CHECK(mliVec_equal_margin(
                intersection.surface_normal, mliVec_set(0, 0, 1), 1e-9));
        CHECK_MARGIN(intersection.distance_of_ray, 3., 1e-9);

        CHECK(scenery.resources.num_media == 2);
        CHECK(scenery.resources.num_functions == 4);

        side_coming_from = _mli_side_coming_from(&scenery, &intersection);
        surf_coming_from = scenery.resources.surfaces[side_coming_from.surface];
        medi_coming_from = scenery.resources.media[side_coming_from.medium];

        side_going_to = _mli_side_going_to(&scenery, &intersection);
        surf_going_to = scenery.resources.surfaces[side_going_to.surface];
        medi_going_to = scenery.resources.media[side_going_to.medium];

        CHECK(surf_going_to.material == MLI_MATERIAL_TRANSPARENT);
        CHECK(surf_going_to.color == 0);
        CHECK(medi_going_to.refraction == 0);
        CHECK(medi_going_to.absorbtion == 2);

        CHECK(surf_coming_from.material == MLI_MATERIAL_TRANSPARENT);
        CHECK(surf_coming_from.color == 1);
        CHECK(medi_coming_from.refraction == 1);
        CHECK(medi_coming_from.absorbtion == 2);

        CHECK(mliDynPhotonInteraction_malloc(&history, max_interactions));

        CHECK(mli_propagate_photon(
                &scenery,
                &octree,
                &history,
                &photon,
                &prng,
                max_interactions));

        CHECK(history.dyn.size >= 1);

        mliDynPhotonInteraction_print(&history, &scenery);

        mliScenery_free(&scenery);
        mliOcTree_free(&octree);
        mliDynPhotonInteraction_free(&history);
}

CASE("Do not leak out of closed box")
{
        struct mliMT19937 prng = mliMT19937_init(0u);
        struct mliDynPhoton photons = mliDynPhoton_init();
        struct mliDynPhotonInteraction history = mliDynPhotonInteraction_init();
        struct mliScenery scenery = mliScenery_init();
        struct mliOcTree octree = mliOcTree_init();
        const float wavelength = 433e-9;
        const float opening_angle = mli_deg2rad(180.0);
        const uint64_t num_photons = 100u;
        const uint64_t max_interactions = 100u;
        const int64_t sensor_id = 3;
        const struct mliVec center_of_crystal = mliVec_set(5e-3, 5e-3, 5e-3);
        uint64_t i;

        CHECK(mliScenery_malloc_from_json_path(
                &scenery,
                "tests/resources/lyso_crystal_scenery.json"));

        CHECK(mliOcTree_malloc_from_scenery(&octree, &scenery));

        CHECK(mliDynPhoton_malloc(&photons, 0u));
        CHECK(photons.dyn.size == 0);
        CHECK(point_like_towards_z_opening_angle_num_photons(
                &photons,
                wavelength,
                opening_angle,
                num_photons,
                &prng));
        CHECK(photons.dyn.size == num_photons);
        for (i = 0; i < photons.dyn.size; i++) {
                photons.arr[i].ray.support = center_of_crystal;
        }

        for (i = 0; i < photons.dyn.size; i++) {
                struct mliPhotonInteraction final;
                struct mliPhoton photon = photons.arr[i];

                CHECK(mliDynPhotonInteraction_malloc(
                        &history,
                        max_interactions));

                CHECK(mli_propagate_photon(
                        &scenery,
                        &octree,
                        &history,
                        &photon,
                        &prng,
                        max_interactions));
                /*
                mliDynPhotonInteraction_print(&history, &scenery);
                */

                final = history.arr[history.dyn.size - 1];
                if (final.object_idx >= 0) {
                        int64_t id_primitive = scenery.user_ids[final.object_idx];
                        if (id_primitive == sensor_id) {
                                struct mliVec point;
                                point = final.position;

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
        mliScenery_free(&scenery);
        mliDynPhoton_free(&photons);
}
