/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_photon_propagation.h"
#include "mli_intersection_and_scenery.h"
#include "../lambertian/lambertian.h"
#include "mli_ray_scenery_query.h"
#include "../chk/chk.h"
#include <math.h>

struct mli_PhotonInteraction mliPhotonInteraction_from_Intersection(
        const int64_t type,
        const struct mliScenery *scenery,
        const struct mliIntersectionSurfaceNormal *isec)
{
        struct mli_PhotonInteraction phia;

        struct mli_boundarylayer_Side side_coming_from, side_going_to;

        side_coming_from = mli_get_side_coming_from(scenery, isec);
        side_going_to = mli_get_side_going_to(scenery, isec);

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

int mli_propagate_photon_phong(
        struct mliEnv *env,
        const struct mliIntersectionSurfaceNormal *isec)
{
        double specular;
        double diffuse;
        double rnd;
        struct mli_boundarylayer_Side side_coming_from =
                mli_get_side_coming_from(env->scenery, isec);

        chk_msg(mli_Func_evaluate(
                        &env->scenery->materials
                                 .surfaces[side_coming_from.surface]
                                 .diffuse_reflection,
                        env->photon->wavelength,
                        &diffuse),
                "Failed to eval. diffuse reflection for wavelength.");
        chk_msg(mli_Func_evaluate(
                        &env->scenery->materials
                                 .surfaces[side_coming_from.surface]
                                 .specular_reflection,
                        env->photon->wavelength,
                        &specular),
                "Failed to eval. specular reflection for wavelength.");
        rnd = mli_Prng_uniform(env->prng);
        /*
                                                      absorbtion
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
                        "Failed to continue after diffuse reflection phong.");
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
                        "Failed to continue after specular reflection phong.");
        } else {
                chk(mli_PhotonInteractionVector_push_back(
                        env->history,
                        mliPhotonInteraction_from_Intersection(
                                MLI_PHOTON_ABSORBTION, env->scenery, isec)));
        }
        return 1;
chk_error:
        return 0;
}

int mli_propagate_photon_pass_boundary_layer(
        struct mliEnv *env,
        const struct mliIntersectionSurfaceNormal *isec,
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
        return 1;
chk_error:
        return 0;
}

int mli_propagate_photon_probability_passing_medium_coming_from(
        const struct mliScenery *scenery,
        const struct mli_Photon *photon,
        const struct mliIntersectionSurfaceNormal *isec,
        double *probability_passing)
{
        double one_over_e_way;
        const struct mli_boundarylayer_Side side_coming_from =
                mli_get_side_coming_from(scenery, isec);
        chk_msg(mli_Func_evaluate(
                        &scenery->materials.media[side_coming_from.medium]
                                 .absorbtion,
                        photon->wavelength,
                        &one_over_e_way),
                "Photon's wavelength is out of range to "
                "evaluate absorbtion in medium coming from");
        (*probability_passing) = exp(-isec->distance_of_ray / one_over_e_way);
        return 1;
chk_error:
        return 0;
}

int mli_propagate_photon_fresnel_refraction_and_reflection(
        struct mliEnv *env,
        const struct mliIntersectionSurfaceNormal *isec)
{
        struct mli_Fresnel fresnel;
        double n_going_to;
        double n_coming_from;
        double reflection_propability;
        struct mli_Vec facing_surface_normal;
        chk_msg(mli_Func_evaluate(
                        mli_get_refractive_index_going_to(env->scenery, isec),
                        env->photon->wavelength,
                        &n_going_to),
                "Failed to eval. refraction going to for wavelength.");
        chk_msg(mli_Func_evaluate(
                        mli_get_refractive_index_coming_from(
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
        return 1;
chk_error:
        return 0;
}

int mli_propagate_photon_interact_with_object(
        struct mliEnv *env,
        const struct mliIntersectionSurfaceNormal *isec)
{
        struct mli_Surface surface_coming_from;
        const struct mli_boundarylayer_Side side_coming_from =
                mli_get_side_coming_from(env->scenery, isec);
        surface_coming_from =
                env->scenery->materials.surfaces[side_coming_from.surface];
        switch (surface_coming_from.material) {
        case MLI_SURFACE_TRANSPARENT:
                chk_msg(mli_propagate_photon_fresnel_refraction_and_reflection(
                                env, isec),
                        "Failed Fresnel.");
                break;
        case MLI_SURFACE_PHONG:
                chk_msg(mli_propagate_photon_phong(env, isec),
                        "Failed Phong-material.");
                break;
        default:
                chk_bad("Unkown material of surface.");
                break;
        }
        return 1;
chk_error:
        return 0;
}

int mli_propagate_photon_distance_until_absorbtion(
        const struct mli_Func *absorbtion_in_medium_passing_through,
        const double wavelength,
        struct mli_Prng *prng,
        double *distance_until_absorbtion)
{
        double one_over_e_way;
        chk_msg(mli_Func_evaluate(
                        absorbtion_in_medium_passing_through,
                        wavelength,
                        &one_over_e_way),
                "Failed to eval. absorbtion for wavelength.");
        (*distance_until_absorbtion) =
                mli_Prng_expovariate(prng, 1. / one_over_e_way);
        return 1;
chk_error:
        return 0;
}

int mli_propagate_photon_work_on_causal_intersection(struct mliEnv *env)
{
        int ray_does_intersect_surface = 0;
        double distance_until_absorbtion = 0.0;
        struct mliIntersectionSurfaceNormal next_intersection;
        struct mli_Func *absorbtion_in_medium_passing_through;
        struct mli_PhotonInteraction phia;

        ray_does_intersect_surface = mli_query_intersection_with_surface_normal(
                env->scenery, env->photon->ray, &next_intersection);

        if (ray_does_intersect_surface) {
                int photon_is_absorbed_before_reaching_surface;
                struct mli_boundarylayer_Side side_coming_from;

                side_coming_from = mli_get_side_coming_from(
                        env->scenery, &next_intersection);
                absorbtion_in_medium_passing_through =
                        &env->scenery->materials.media[side_coming_from.medium]
                                 .absorbtion;
                chk(mli_propagate_photon_distance_until_absorbtion(
                        absorbtion_in_medium_passing_through,
                        env->photon->wavelength,
                        env->prng,
                        &distance_until_absorbtion));

                photon_is_absorbed_before_reaching_surface =
                        distance_until_absorbtion <
                        next_intersection.distance_of_ray;

                if (env->history->size == 0) {
                        /* creation */
                        phia.type = MLI_PHOTON_CREATION;
                        phia.position = env->photon->ray.support;
                        phia.position_local = phia.position;
                        phia.distance_of_ray = 0.0;
                        phia.on_geometry_surface = 0;
                        phia.geometry_id = mliGeometryId_init();
                        phia.from_outside_to_inside = 1;

                        phia.medium_coming_from = side_coming_from.medium;
                        phia.medium_going_to = side_coming_from.medium;

                        chk(mli_PhotonInteractionVector_push_back(
                                env->history, phia));
                }

                if (photon_is_absorbed_before_reaching_surface) {
                        /* absorbtion in medium */
                        phia.type = MLI_PHOTON_ABSORBTION_MEDIUM;
                        phia.position = mli_Ray_at(
                                &env->photon->ray, distance_until_absorbtion);
                        ;
                        phia.position_local = phia.position;
                        phia.distance_of_ray = distance_until_absorbtion;
                        phia.on_geometry_surface = 0;
                        phia.geometry_id = mliGeometryId_init();
                        phia.from_outside_to_inside = 1;

                        phia.medium_coming_from = side_coming_from.medium;
                        phia.medium_going_to = side_coming_from.medium;

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

                absorbtion_in_medium_passing_through =
                        &env->scenery->materials.media[default_medium]
                                 .absorbtion;
                chk(mli_propagate_photon_distance_until_absorbtion(
                        absorbtion_in_medium_passing_through,
                        env->photon->wavelength,
                        env->prng,
                        &distance_until_absorbtion));

                if (env->history->size == 0) {
                        /* creation */
                        phia.type = MLI_PHOTON_CREATION;
                        phia.position = env->photon->ray.support;
                        phia.position_local = phia.position;
                        phia.distance_of_ray = 0.0;
                        phia.on_geometry_surface = 0;
                        phia.geometry_id = mliGeometryId_init();
                        phia.from_outside_to_inside = 1;

                        phia.medium_coming_from = default_medium;
                        phia.medium_going_to = default_medium;

                        chk(mli_PhotonInteractionVector_push_back(
                                env->history, phia));
                }

                /* absorbtion in medium */
                phia.type = MLI_PHOTON_ABSORBTION_MEDIUM;
                phia.position = mli_Ray_at(
                        &env->photon->ray, distance_until_absorbtion);
                phia.position_local = phia.position;
                phia.distance_of_ray = distance_until_absorbtion;
                phia.on_geometry_surface = 0;
                phia.geometry_id = mliGeometryId_init();
                phia.from_outside_to_inside = 1;

                phia.medium_coming_from = default_medium;
                phia.medium_going_to = default_medium;

                chk(mli_PhotonInteractionVector_push_back(env->history, phia));
        }

        return 1;
chk_error:
        return 0;
}

int mli_propagate_photon_env(struct mliEnv *env)
{
        if (env->max_interactions > env->history->size) {
                chk_msg(mli_propagate_photon_work_on_causal_intersection(env),
                        "Failed to work on intersection.");
        }
        return 1;
chk_error:
        return 0;
}

int mli_propagate_photon(
        const struct mliScenery *scenery,
        struct mli_PhotonInteractionVector *history,
        struct mli_Photon *photon,
        struct mli_Prng *prng,
        const uint64_t max_interactions)
{
        struct mliEnv env;
        env.scenery = scenery;
        env.history = history;
        env.photon = photon;
        env.prng = prng;
        env.max_interactions = max_interactions;
        chk(mli_propagate_photon_env(&env));
        return 1;
chk_error:
        return 0;
}
