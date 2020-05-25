/* Copyright 2019 Sebastian Achim Mueller                                     */

CASE("simple propagation")
{
        struct mliMT19937 prng = mliMT19937_init(0u);
        struct mliScenery scenery = mliScenery_init();
        struct mliOcTree octree = mliOcTree_init();
        struct mliDynPhotonInteraction history = mliDynPhotonInteraction_init();
        /*struct mliPhotonInteraction creation;*/
        struct mliIntersection intersection;
        struct mliSide side_coming_from, side_going_to;
        struct mliSurface surf_coming_from, surf_going_to;
        struct mliMedium medi_coming_from, medi_going_to;
        size_t max_interactions = 16;

        struct mliPhoton photon;
        photon.ray = mliRay_set(mliVec_set(0, 0, -3), mliVec_set(0, 0, 1));
        photon.wavelength = 600e-9;
        photon.simulation_truth_id = 0;

        CHECK(mliScenery_malloc_from_json_path(
                &scenery, "tests/resources/glass_cylinder_in_air.json"));
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

        /*
        creation.type = MLI_PHOTON_CREATION;
        creation.position = photon.ray.support;
        creation.refraction_going_to = 0u;
        creation.absorbtion_going_to = 2u;
        creation.refraction_coming_from = 0u;
        creation.absorbtion_coming_from = 2u;
        creation.object_idx = -1;
        creation.from_outside_to_inside = 1;
        CHECK(mliDynPhotonInteraction_push_back(&history, creation));
        */

        CHECK(mli_propagate_photon(
                &scenery,
                &octree,
                &history,
                &photon,
                &prng,
                max_interactions));

        CHECK(history.dyn.size >= 1);

        mliDynPhotonInteraction_print(&history);

        mliScenery_free(&scenery);
        mliOcTree_free(&octree);
        mliDynPhotonInteraction_free(&history);
}
