/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_PHOTON_PROPAGATION_H_
#define MERLICT_C89_MLI_PHOTON_PROPAGATION_H_

#include <stdint.h>

#include "mliPhoton.h"
#include "mliPhotonInteraction.h"
#include "mliScenery.h"
#include "mliDynPhotonInteraction.h"
#include "mli_random.h"

struct mliEnv {
        const struct mliScenery *scenery;
        struct mliDynPhotonInteraction *history;
        struct mliPhoton *photon;
        struct mliPrng *prng;
        uint64_t max_interactions;
};

int mli_propagate_photon(
        const struct mliScenery *scenery,
        struct mliDynPhotonInteraction *history,
        struct mliPhoton *photon,
        struct mliPrng *prng,
        const uint64_t max_interactions);
int _mli_work_on_causal_intersection(struct mliEnv *env);
int _mli_distance_until_absorbtion(
        const struct mliFunc *absorbtion_in_medium_passing_through,
        const double wavelength,
        struct mliPrng *prng,
        double *distance_until_absorbtion);
int _mli_interact_with_object(
        struct mliEnv *env,
        const struct mliIntersectionSurfaceNormal *isec);
int _mli_fresnel_refraction_and_reflection(
        struct mliEnv *env,
        const struct mliIntersectionSurfaceNormal *isec);
int _mli_probability_passing_medium_coming_from(
        const struct mliScenery *scenery,
        const struct mliPhoton *photon,
        const struct mliIntersectionSurfaceNormal *isec,
        double *probability_passing);
int _mli_pass_boundary_layer(
        struct mliEnv *env,
        const struct mliIntersectionSurfaceNormal *isec,
        const struct mliFresnel fresnel);
int _mli_phong(
        struct mliEnv *env,
        const struct mliIntersectionSurfaceNormal *isec);
struct mliPhotonInteraction mliPhotonInteraction_from_Intersection(
        const int64_t type,
        const struct mliScenery *scenery,
        const struct mliIntersectionSurfaceNormal *isec);
int _mli_propagate_photon(struct mliEnv *env);
#endif
