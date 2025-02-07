/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "photon_propagation.h"
#include "../raytracing/intersection_and_scenery.h"
#include "../lambertian/lambertian.h"
#include "../raytracing/ray_scenery_query.h"
#include "../chk/chk.h"
#include <math.h>

struct mli_PhotonInteraction mliPhotonInteraction_from_Intersection(
        const int64_t type,
        const struct mli_Scenery *scenery,
        const struct mli_IntersectionSurfaceNormal *isec)
{
        struct mli_PhotonInteraction phia;

        struct mli_BoundaryLayer_Side side_coming_from, side_going_to;

        side_coming_from = mli_raytracing_get_side_coming_from(scenery, isec);
        side_going_to = mli_raytracing_get_side_going_to(scenery, isec);

        phia.type = type;
        phia.position = isec->position;
        phia.position_local = isec->position_local;
        phia.medium_coming_from = side_coming_from.medium;
        phia.medium_going_to = side_going_to.medium;
        phia.distance_of_ray = isec->distance_of_ray;
        phia.on_geometry_surface = 1;
        phia.geometry_id = isec->geometry_id;
        phia.from_outside_to_inside = isec->from_outside_to_inside;
        return phia;
}

chk_rc mli_propagate_photon_cooktorrance(
        struct mli_PhotonPropagation *env,
        const struct mli_IntersectionSurfaceNormal *isec)
{
        double spectral_reflection_propability;
        double diffuse;
        double specular;

        double rnd;
        struct mli_BoundaryLayer_Side side_coming_from =
                mli_raytracing_get_side_coming_from(env->scenery, isec);

        const struct mli_Surface_CookTorrance *cook =
                &env->scenery->materials.surfaces
                         .array[side_coming_from.surface]
                         .data.cooktorrance;
        const struct mli_Func *reflection_spectrum =
                &env->scenery->materials.spectra
                         .array[cook->reflection_spectrum]
                         .spectrum;

        chk_msg(mli_Func_evaluate(
                        reflection_spectrum,
                        env->photon->wavelength,
                        &spectral_reflection_propability),
                "Failed to eval. spectral_reflection_propability for "
                "wavelength.");

        diffuse = cook->diffuse_weight * spectral_reflection_propability;
        specular = cook->specular_weight * spectral_reflection_propability;

        rnd = mli_Prng_uniform(env->prng);
        /*
                                                      absorption
                  diffuse      specular        (1.0 - diffuse - specular)
                  __/\____  _____/\__________  ___________/\____________
                 /        \/                 \/                         \

                |.........|..................|..........................|
                |         |                  |                          |
               0.0      diffuse      diffuse + specular                1.0
        */
        if (rnd < diffuse) {
                chk(mli_PhotonInteractionVector_push_back(
                        env->history,
                        mliPhotonInteraction_from_Intersection(
                                MLI_PHOTON_DIFFUSE_REFLECTION,
                                env->scenery,
                                isec)));
                env->photon->ray = mli_Ray_set(
                        isec->position,
                        mli_lambertian_cosine_law_draw_direction_wrt_surface_normal(
                                env->prng, isec->surface_normal));
                chk_msg(mli_propagate_photon_env(env),
                        "Failed to continue after diffuse reflection "
                        "cooktorrance.");
        } else if (rnd < (specular + diffuse)) {
                chk(mli_PhotonInteractionVector_push_back(
                        env->history,
                        mliPhotonInteraction_from_Intersection(
                                MLI_PHOTON_SPECULAR_REFLECTION,
                                env->scenery,
                                isec)));
                env->photon->ray = mli_Ray_set(
                        isec->position,
                        mli_Vec_mirror(
                                env->photon->ray.direction,
                                isec->surface_normal));
                chk_msg(mli_propagate_photon_env(env),
                        "Failed to continue after specular reflection "
                        "cooktorrance.");
        } else {
                chk(mli_PhotonInteractionVector_push_back(
                        env->history,
                        mliPhotonInteraction_from_Intersection(
                                MLI_PHOTON_ABSORPTION, env->scenery, isec)));
        }
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_propagate_photon_pass_boundary_layer(
        struct mli_PhotonPropagation *env,
        const struct mli_IntersectionSurfaceNormal *isec,
        const struct mli_Fresnel fresnel)
{
        chk(mli_PhotonInteractionVector_push_back(
                env->history,
                mliPhotonInteraction_from_Intersection(
                        MLI_PHOTON_REFRACTION, env->scenery, isec)));
        env->photon->ray = mli_Ray_set(
                isec->position, mli_Fresnel_refraction_direction(fresnel));
        chk_msg(mli_propagate_photon_env(env),
                "Failed to continue after passing boundary layer");
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_propagate_photon_probability_passing_medium_coming_from(
        const struct mli_Scenery *scenery,
        const struct mli_Photon *photon,
        const struct mli_IntersectionSurfaceNormal *isec,
        double *probability_passing)
{
        const struct mli_BoundaryLayer_Side side_coming_from =
                mli_raytracing_get_side_coming_from(scenery, isec);
        const struct mli_Medium *medium_coming_from =
                &scenery->materials.media.array[side_coming_from.medium];

        const struct mli_Func *absorption_spectrum =
                &scenery->materials.spectra
                         .array[medium_coming_from->absorption_spectrum]
                         .spectrum;
        double absorption_coefficient;

        chk_msg(mli_Func_evaluate(
                        absorption_spectrum,
                        photon->wavelength,
                        &absorption_coefficient),
                "Photon's wavelength is out of range to "
                "evaluate absorption in medium coming from");
        (*probability_passing) =
                exp(-isec->distance_of_ray * absorption_coefficient);

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_propagate_photon_fresnel_refraction_and_reflection(
        struct mli_PhotonPropagation *env,
        const struct mli_IntersectionSurfaceNormal *isec)
{
        struct mli_Fresnel fresnel;
        double n_going_to;
        double n_coming_from;
        double reflection_propability;
        struct mli_Vec facing_surface_normal;
        chk_msg(mli_Func_evaluate(
                        mli_raytracing_get_refractive_index_going_to(
                                env->scenery, isec),
                        env->photon->wavelength,
                        &n_going_to),
                "Failed to eval. refraction going to for wavelength.");
        chk_msg(mli_Func_evaluate(
                        mli_raytracing_get_refractive_index_coming_from(
                                env->scenery, isec),
                        env->photon->wavelength,
                        &n_coming_from),
                "Failed to eval. refraction coming from for wavelength.");
        facing_surface_normal =
                isec->from_outside_to_inside
                        ? isec->surface_normal
                        : mli_Vec_multiply(isec->surface_normal, -1.0);
        fresnel = mli_Fresnel_init(
                env->photon->ray.direction,
                facing_surface_normal,
                n_coming_from,
                n_going_to);
        reflection_propability = mli_Fresnel_reflection_propability(fresnel);
        if (reflection_propability > mli_Prng_uniform(env->prng)) {
                chk(mli_PhotonInteractionVector_push_back(
                        env->history,
                        mliPhotonInteraction_from_Intersection(
                                MLI_PHOTON_FRESNEL_REFLECTION,
                                env->scenery,
                                isec)));
                env->photon->ray = mli_Ray_set(
                        isec->position,
                        mli_Fresnel_reflection_direction(fresnel));
                chk_msg(mli_propagate_photon_env(env),
                        "Failed to continue after reflection");
        } else {
                chk_msg(mli_propagate_photon_pass_boundary_layer(
                                env, isec, fresnel),
                        "Failed to pass boundary");
        }
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_propagate_photon_interact_with_object(
        struct mli_PhotonPropagation *env,
        const struct mli_IntersectionSurfaceNormal *isec)
{
        const struct mli_BoundaryLayer_Side side_coming_from =
                mli_raytracing_get_side_coming_from(env->scenery, isec);
        const struct mli_Surface *surface_coming_from =
                &env->scenery->materials.surfaces
                         .array[side_coming_from.surface];

        switch (surface_coming_from->type) {
        case MLI_SURFACE_TYPE_TRANSPARENT:
                chk_msg(mli_propagate_photon_fresnel_refraction_and_reflection(
                                env, isec),
                        "Failed Fresnel.");
                break;
        case MLI_SURFACE_TYPE_COOKTORRANCE:
                chk_msg(mli_propagate_photon_cooktorrance(env, isec),
                        "Failed cook-torrance.");
                break;
        default:
                chk_bad("Unkown type of surface.");
                break;
        }
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_propagate_photon_distance_until_absorption(
        const struct mli_Func *absorption_in_medium_passing_through,
        const double wavelength,
        struct mli_Prng *prng,
        double *distance_until_absorption)
{
        double absorption_coefficient;
        chk_msg(mli_Func_evaluate(
                        absorption_in_medium_passing_through,
                        wavelength,
                        &absorption_coefficient),
                "Failed to eval. absorption for wavelength.");
        (*distance_until_absorption) =
                mli_Prng_expovariate(prng, absorption_coefficient);
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_propagate_photon_work_on_causal_intersection(
        struct mli_PhotonPropagation *env)
{
        int ray_does_intersect_surface = 0;
        double distance_until_absorption = 0.0;
        struct mli_IntersectionSurfaceNormal next_intersection;
        struct mli_Func *absorption_in_medium_passing_through;
        struct mli_Medium *medium_passing_through;
        struct mli_PhotonInteraction phia;

        ray_does_intersect_surface =
                mli_raytracing_query_intersection_with_surface_normal(
                        env->scenery, env->photon->ray, &next_intersection);

        if (ray_does_intersect_surface) {
                int photon_is_absorbed_before_reaching_surface;
                struct mli_IntersectionLayer layer;

                layer = mli_raytracing_get_intersection_layer(
                        env->scenery, &next_intersection);

                absorption_in_medium_passing_through =
                        &env->scenery->materials.spectra
                                 .array[layer.side_coming_from.medium
                                                ->absorption_spectrum]
                                 .spectrum;

                chk(mli_propagate_photon_distance_until_absorption(
                        absorption_in_medium_passing_through,
                        env->photon->wavelength,
                        env->prng,
                        &distance_until_absorption));

                photon_is_absorbed_before_reaching_surface =
                        distance_until_absorption <
                        next_intersection.distance_of_ray;

                if (env->history->size == 0) {
                        /* creation */
                        phia.type = MLI_PHOTON_CREATION;
                        phia.position = env->photon->ray.support;
                        phia.position_local = phia.position;
                        phia.distance_of_ray = 0.0;
                        phia.on_geometry_surface = 0;
                        phia.geometry_id = mli_GeometryId_init();
                        phia.from_outside_to_inside = 1;

                        phia.medium_coming_from =
                                layer.side_coming_from.medium_idx;
                        phia.medium_going_to =
                                layer.side_coming_from.medium_idx;

                        chk(mli_PhotonInteractionVector_push_back(
                                env->history, phia));
                }

                if (photon_is_absorbed_before_reaching_surface) {
                        /* absorption in medium */
                        phia.type = MLI_PHOTON_ABSORPTION_MEDIUM;
                        phia.position = mli_Ray_at(
                                &env->photon->ray, distance_until_absorption);
                        ;
                        phia.position_local = phia.position;
                        phia.distance_of_ray = distance_until_absorption;
                        phia.on_geometry_surface = 0;
                        phia.geometry_id = mli_GeometryId_init();
                        phia.from_outside_to_inside = 1;

                        phia.medium_coming_from =
                                layer.side_coming_from.medium_idx;
                        phia.medium_going_to =
                                layer.side_coming_from.medium_idx;

                        chk(mli_PhotonInteractionVector_push_back(
                                env->history, phia));
                } else {
                        chk_msg(mli_propagate_photon_interact_with_object(
                                        env, &next_intersection),
                                "Failed to interact photon with object "
                                "surface.");
                }
        } else {
                const uint64_t default_medium =
                        env->scenery->materials.default_medium;

                medium_passing_through =
                        &env->scenery->materials.media.array[default_medium];
                absorption_in_medium_passing_through =
                        &env->scenery->materials.spectra
                                 .array[medium_passing_through
                                                ->absorption_spectrum]
                                 .spectrum;

                chk(mli_propagate_photon_distance_until_absorption(
                        absorption_in_medium_passing_through,
                        env->photon->wavelength,
                        env->prng,
                        &distance_until_absorption));

                if (env->history->size == 0) {
                        /* creation */
                        phia.type = MLI_PHOTON_CREATION;
                        phia.position = env->photon->ray.support;
                        phia.position_local = phia.position;
                        phia.distance_of_ray = 0.0;
                        phia.on_geometry_surface = 0;
                        phia.geometry_id = mli_GeometryId_init();
                        phia.from_outside_to_inside = 1;

                        phia.medium_coming_from = default_medium;
                        phia.medium_going_to = default_medium;

                        chk(mli_PhotonInteractionVector_push_back(
                                env->history, phia));
                }

                /* absorption in medium */
                phia.type = MLI_PHOTON_ABSORPTION_MEDIUM;
                phia.position = mli_Ray_at(
                        &env->photon->ray, distance_until_absorption);
                phia.position_local = phia.position;
                phia.distance_of_ray = distance_until_absorption;
                phia.on_geometry_surface = 0;
                phia.geometry_id = mli_GeometryId_init();
                phia.from_outside_to_inside = 1;

                phia.medium_coming_from = default_medium;
                phia.medium_going_to = default_medium;

                chk(mli_PhotonInteractionVector_push_back(env->history, phia));
        }

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_propagate_photon_env(struct mli_PhotonPropagation *env)
{
        if (env->max_interactions > env->history->size) {
                chk_msg(mli_propagate_photon_work_on_causal_intersection(env),
                        "Failed to work on intersection.");
        }
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_propagate_photon(
        const struct mli_Scenery *scenery,
        struct mli_PhotonInteractionVector *history,
        struct mli_Photon *photon,
        struct mli_Prng *prng,
        const uint64_t max_interactions)
{
        struct mli_PhotonPropagation env;
        env.scenery = scenery;
        env.history = history;
        env.photon = photon;
        env.prng = prng;
        env.max_interactions = max_interactions;
        chk(mli_propagate_photon_env(&env));
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}
