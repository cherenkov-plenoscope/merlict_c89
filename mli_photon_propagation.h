/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLI_PHOTON_PROPAGATION_H_
#define MERLICT_MLI_PHOTON_PROPAGATION_H_

#include <math.h>
#include <stdint.h>
#include "mliPhoton.h"
#include "mliScenery.h"
#include "mliOcTree.h"
#include "mliPhotonHistory.h"
#include "mliMT19937.h"
#include "mli_intersection_and_scenery.h"

typedef struct {
    const mliScenery *scenery;
    const mliOcTree* octree;
    mliPhotonHistory* history;
    mliPhoton *photon;
    mliMT19937 *prng;
} mliEnv;

int _mli_propagate_photon(mliEnv *env);

mliPhotonInteraction mliPhotonInteraction_from_Intersection(
    const int64_t type,
    const mliScenery *scenery,
    const mliIntersection *isec) {
    mliPhotonInteraction phia;
    mliSurfaces surfaces;
    mliSurface surf_coming_from, surf_going_to;
    surfaces = mliScenery_object_surfaces(scenery, isec->object_idx);
    if (isec->from_outside_to_inside) {
        surf_coming_from = scenery->surfaces[surfaces.outer];
        surf_going_to = scenery->surfaces[surfaces.inner];
    } else {
        surf_coming_from = scenery->surfaces[surfaces.inner];
        surf_going_to = scenery->surfaces[surfaces.outer];
    }
    phia.type = type;
    phia.position = isec->position;
    phia.refraction_coming_from = surf_coming_from.medium_refraction;
    phia.refraction_going_to = surf_going_to.medium_refraction;
    phia.absorbtion_coming_from = surf_coming_from.medium_absorbtion;
    phia.absorbtion_going_to = surf_going_to.medium_absorbtion;
    phia.distance_of_ray = isec->distance_of_ray;
    phia._object_idx = isec->object_idx;
    phia._from_outside_to_inside = isec->from_outside_to_inside;
    return phia;}

int _mli_phong(
    mliEnv* env,
    const mliIntersection *isec) {
    double specular;
    double diffuse;
    double rnd;
    mliSurface surface = _mli_surface_coming_from(env->scenery, isec);
    mli_check(mliFunc_evaluate(
        &env->scenery->functions[surface.boundary_layer_diffuse_reflection],
        env->photon->wavelength,
        &diffuse),
        "Failed to eval. diffuse reflection for wavelength.");
    mli_check(mliFunc_evaluate(
        &env->scenery->functions[surface.boundary_layer_specular_reflection],
        env->photon->wavelength,
        &specular),
        "Failed to eval. specular reflection for wavelength.");
    rnd = mliMT19937_uniform(env->prng);
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
        const uint64_t n = env->history->num;
        env->photon->ray = mliRay_set(
            isec->position,
            mli_draw_lambertian_direction_wrt_surface_normal(
                env->prng,
                isec->surface_normal));
        env->history->actions[n] = mliPhotonInteraction_from_Intersection(
            MLI_PHOTON_DIFFUSE_REFLECTION,
            env->scenery,
            isec);
        env->history->num += 1;
        mli_check(
            _mli_propagate_photon(env),
            "Failed to continue after diffuse reflection phong.");
    } else if (rnd < (specular + diffuse)) {
        const uint64_t n = env->history->num;
        env->photon->ray = mliRay_set(
            isec->position,
            mliVec_mirror(env->photon->ray.direction, isec->surface_normal));
        env->history->actions[n] = mliPhotonInteraction_from_Intersection(
            MLI_PHOTON_SPECULAR_REFLECTION,
            env->scenery,
            isec);
        env->history->num += 1;
        mli_check(
            _mli_propagate_photon(env),
            "Failed to continue after specular reflection phong.");
    } else {
        const uint64_t n = env->history->num;
        env->history->actions[n] = mliPhotonInteraction_from_Intersection(
            MLI_PHOTON_ABSORBTION,
            env->scenery,
            isec);
        env->history->num += 1;
    }
    return 1;
error:
    return 0;}

int _mli_pass_boundary_layer(
    mliEnv *env,
    const mliIntersection *isec,
    const mliFresnel fresnel) {
    uint64_t n = env->history->num;
    env->history->actions[n] = mliPhotonInteraction_from_Intersection(
        MLI_PHOTON_REFRACTION,
        env->scenery,
        isec);
    env->history->num += 1;
    env->photon->ray = mliRay_set(
        isec->position,
        mliFresnel_refraction_direction(fresnel));
    mli_check(
        _mli_propagate_photon(env),
        "Failed to continue after passing boundary layer");
    return 1;
error:
    return 0;}

int _mli_probability_passing_medium_coming_from(
    const mliScenery *scenery,
    const mliPhoton* photon,
    const mliIntersection *isec,
    double *probability_passing) {
    const mliSurface surface_coming_from = _mli_surface_coming_from(
        scenery,
        isec);
    double one_over_e_way;
    mli_check(mliFunc_evaluate(
        &scenery->functions[surface_coming_from.medium_absorbtion],
        photon->wavelength,
        &one_over_e_way),
        "Photon's wavelength is out of range to "
        "evaluate absorbtion in medium coming from");
     (*probability_passing) = exp(-isec->distance_of_ray/one_over_e_way);
    return 1;
error:
    return 0;}

int _mli_fresnel_refraction_and_reflection(
    mliEnv *env,
    const mliIntersection *isec) {
    mliFresnel fresnel;
    double n_going_to;
    double n_coming_from;
    double reflection_propability;
    struct mliVec facing_surface_normal;
    mli_check(mliFunc_evaluate(
        _mli_refractive_index_going_to(env->scenery, isec),
        env->photon->wavelength,
        &n_going_to),
        "Failed to eval. refraction going to for wavelength.");
    mli_check(mliFunc_evaluate(
        _mli_refractive_index_coming_from(env->scenery, isec),
        env->photon->wavelength,
        &n_coming_from),
        "Failed to eval. refraction coming from for wavelength.");
    facing_surface_normal = isec->from_outside_to_inside ?
        isec->surface_normal : mliVec_multiply(isec->surface_normal, -1.0);
    fresnel = mliFresnel_init(
        env->photon->ray.direction,
        facing_surface_normal,
        n_coming_from,
        n_going_to);
    reflection_propability = mliFresnel_reflection_propability(fresnel);
    if (reflection_propability > mliMT19937_uniform(env->prng)) {
        uint64_t n;
        env->photon->ray = mliRay_set(
            isec->position,
            mliFresnel_reflection_direction(fresnel));
        n = env->history->num + 1;
        env->history->actions[n] = mliPhotonInteraction_from_Intersection(
            MLI_PHOTON_FRESNEL_REFLECTION,
            env->scenery,
            isec);
        env->history->num += 1;
        mli_check(
            _mli_propagate_photon(env),
            "Failed to continue after reflection");
    } else {
        mli_check(
            _mli_pass_boundary_layer(env, isec, fresnel),
            "Failed to pass boundary");
    }
    return 1;
error:
    return 0;}

int _mli_interact_with_object(mliEnv *env, const mliIntersection *isec) {
    const mliSurface surface_coming_from = _mli_surface_coming_from(
        env->scenery,
        isec);
    switch (surface_coming_from.material) {
        case MLI_MATERIAL_TRANSPARENT:
            mli_check(
                _mli_fresnel_refraction_and_reflection(env, isec),
                "Failed Fresnel.");
            break;
        case MLI_MATERIAL_PHONG:
            mli_check(
                _mli_phong(env, isec),
                "Failed Phong-material.");
            break;
        default:
            mli_sentinel("Unkown material of surface.");
            break;
    }
    return 1;
error:
    return 0;}

int _mli_distance_until_next_absorbtion(
    mliEnv *env,
    double *distance_to_next_absorption){
    double one_over_e_way;
    uint64_t last_interaction = env->history->num - 1;
    mliFunc* absorbtion_in_medium_coming_from = &env->scenery->functions[
            env->history->actions[last_interaction].absorbtion_going_to];
    mli_check(mliFunc_evaluate(
        absorbtion_in_medium_coming_from,
        env->photon->wavelength,
        &one_over_e_way),
        "Failed to eval. absorbtion in medium coming from for wavelength.");
    (*distance_to_next_absorption) = mliMT19937_expovariate(
        env->prng,
        1./one_over_e_way);
    return 1;
error:
    return 0;}

int _mli_work_on_causal_intersection(mliEnv *env) {
    int hit;
    mliIntersection isec;
    double distance_until_next_absorbtion;
    hit = mli_first_casual_intersection(
        env->scenery,
        env->octree,
        env->photon->ray,
        &isec);
    mli_check(_mli_distance_until_next_absorbtion(
        env,
        &distance_until_next_absorbtion),
        "Failed to estimate photon's probability to pass medium coming from.");
    if (hit && distance_until_next_absorbtion > isec.distance_of_ray) {
        mli_check(
            _mli_interact_with_object(env, &isec),
            "Failed to interact photon with object surface.");
    } else {
        uint64_t n;
        fprintf(stderr, "%s:%d absorb in void\n", __FILE__, __LINE__);
        n = env->history->num + 1;
        env->history->actions[n].type = MLI_PHOTON_ABSORBTION;
        env->history->actions[n].position = mliRay_at(
            &env->photon->ray,
            distance_until_next_absorbtion);
        env->history->actions[n].refraction_coming_from =
            env->history->actions[n-1].refraction_coming_from;
        env->history->actions[n].refraction_going_to =
            env->history->actions[n-1].refraction_going_to;
        env->history->actions[n].absorbtion_coming_from =
            env->history->actions[n-1].absorbtion_coming_from;
        env->history->actions[n].absorbtion_going_to =
            env->history->actions[n-1].absorbtion_going_to;
        env->history->actions[n].distance_of_ray =
            distance_until_next_absorbtion;
        env->history->actions[n]._object_idx = -1;
        env->history->actions[n]._from_outside_to_inside = 1;
        env->history->num += 1;
    }
    return 1;
error:
    return 0;}

int _mli_propagate_photon(mliEnv *env) {
    if(env->history->num_reserved > env->history->num) {
        mli_check(_mli_work_on_causal_intersection(env),
            "Failed to work on intersection.");
    }
    return 1;
error:
    return 0;}

int mli_propagate_photon(
    mliScenery* scenery,
    mliOcTree* octree,
    mliPhotonHistory* history,
    mliPhoton* photon,
    mliMT19937* prng)
{
    mliEnv env;
    env.scenery = scenery;
    env.octree = octree;
    env.history = history;
    env.photon = photon;
    env.prng = prng;
    mli_c(_mli_propagate_photon(&env));
    return 1;
error:
    return 0;}

#endif
