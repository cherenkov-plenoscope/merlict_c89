/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_PHOTON_PROPAGATION_H_
#define MLI_PHOTON_PROPAGATION_H_

#include <stdint.h>

#include "mliPhoton.h"
#include "mliPhotonInteraction.h"
#include "mliScenery.h"
#include "mliDynPhotonInteraction.h"
#include "mli_random.h"
#include "mliFresnel.h"

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
int mli_propagate_photon_work_on_causal_intersection(struct mliEnv *env);
int mli_propagate_photon_distance_until_absorbtion(
        const struct mliFunc *absorbtion_in_medium_passing_through,
        const double wavelength,
        struct mliPrng *prng,
        double *distance_until_absorbtion);
int mli_propagate_photon_interact_with_object(
        struct mliEnv *env,
        const struct mliIntersectionSurfaceNormal *isec);
int mli_propagate_photon_fresnel_refraction_and_reflection(
        struct mliEnv *env,
        const struct mliIntersectionSurfaceNormal *isec);
int mli_propagate_photon_probability_passing_medium_coming_from(
        const struct mliScenery *scenery,
        const struct mliPhoton *photon,
        const struct mliIntersectionSurfaceNormal *isec,
        double *probability_passing);
int mli_propagate_photon_pass_boundary_layer(
        struct mliEnv *env,
        const struct mliIntersectionSurfaceNormal *isec,
        const struct mliFresnel fresnel);
int mli_propagate_photon_phong(
        struct mliEnv *env,
        const struct mliIntersectionSurfaceNormal *isec);
struct mliPhotonInteraction mliPhotonInteraction_from_Intersection(
        const int64_t type,
        const struct mliScenery *scenery,
        const struct mliIntersectionSurfaceNormal *isec);
int mli_propagate_photon_env(struct mliEnv *env);
#endif
