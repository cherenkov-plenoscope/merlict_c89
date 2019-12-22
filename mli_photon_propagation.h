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

typedef struct {
    const mliScenery *scenery;
    const mliOcTree* octree;
    mliPhotonHistory* history;
    mliPhoton *photon;
    mliMT19937 *prng;
} mliEnv;

mliSurface _mli_surface_coming_from(mliEnv *env, const mliIntersection *isec) {
    mliSurfaces surfaces = mliScenery_object_surfaces(
        env->scenery,
        isec->object_idx);
    if (isec->from_outside_to_inside)
        return env->scenery->surfaces[surfaces.inner];
    else
        return env->scenery->surfaces[surfaces.outer];
}

mliSurface _mli_surface_going_to(mliEnv *env, const mliIntersection *isec) {
    mliSurfaces surfaces = mliScenery_object_surfaces(
        env->scenery,
        isec->object_idx);
    if (isec->from_outside_to_inside)
        return env->scenery->surfaces[surfaces.outer];
    else
        return env->scenery->surfaces[surfaces.inner];
}

/*
int _mli_probability_passing_medium_coming_from(
    mliEnv *env,
    const mliIntersection *isec,
    double *probability_passing)
{
    const mliSurface surface_coming_from = _mli_surface_coming_from(env, isec);
    mli_check(mliFunc_evaluate(
        &env->scenery->functions[surface_coming_from.medium_absorbtion],
        env->photon->wavelength,
        probability_passing),
        "Photon's wavelength is out of range to "
        "evaluate absorbtion in medium coming from");
    return 1;
  error:
    return 0;
}


int _mli_absorbed_in_medium_before_reaching_surface(
    mliEnv *env,
    const mliIntersection *isec)
{
    double probability_passing;

    const mliSurface surface_coming_from = _mli_surface_coming_from(env, isec);
    double survival_prob;
    double one_over_e_way;
    mliFunc_evaluate(
        &env->scenery->functions[surface_coming_from.medium_absorbtion],
        env->photon->wavelength,
        &one_over_e_way);

    isec.half_way_depth_coming_from(env->photon->wavelength);
    survival_prob = exp(-isec->distance_of_ray/one_over_e_way);
    flag = mliMT19937_uniform(env->prng) > survival_prob;
    return 1;
  error:
    return 0;}


int _mli_work_on_causal_intersection(mliEnv *env) {
    mliIntersection isec;
    const int causal = mli_first_casual_intersection(
        env->scenery,
        env->octree,
        env->photon->ray,
        &isec);

    double survival_probability;
    mli_check(
        _mli_probability_passing_medium_coming_from(
            env,
            isec,
            &survival_probability),
        "Wha?");

    const int passing = mliMT19937_uniform(env->prng) > survival_probability;

    if (causal && !passing) {
        return 1;
    } else {
        return 1;
    }

    return 1;
error:
    return 0;}

int mli_propagate_photon(mliEnv *env) {
    if(env->history->num_reserved < env->history->num) {
        mli_check(
            _mli_work_on_causal_intersection(env)
            "Failed to work on causal intersection.");
    }
    return 1;
error:
    return 0;}
*/

#endif
