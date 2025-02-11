/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_PHOTON_PROPAGATION_H_
#define MLI_PHOTON_PROPAGATION_H_

#include <stdint.h>

#include "../chk/chk.h"
#include "../photon/photon.h"
#include "../photon/photon_interaction.h"
#include "../scenery/scenery.h"
#include "../photon/photon_interaction_vector.h"
#include "../prng/prng.h"
#include "../fresnel/fresnel.h"

struct mli_PhotonPropagation {
        const struct mli_Scenery *scenery;
        struct mli_PhotonInteractionVector *history;
        struct mli_Photon *photon;
        struct mli_Prng *prng;
        uint64_t max_interactions;
};

chk_rc mli_propagate_photon(
        const struct mli_Scenery *scenery,
        struct mli_PhotonInteractionVector *history,
        struct mli_Photon *photon,
        struct mli_Prng *prng,
        const uint64_t max_interactions);
chk_rc mli_propagate_photon_work_on_causal_intersection(
        struct mli_PhotonPropagation *env);
chk_rc mli_propagate_photon_distance_until_absorption(
        const struct mli_Func *absorption_in_medium_passing_through,
        const double wavelength,
        struct mli_Prng *prng,
        double *distance_until_absorption);
chk_rc mli_propagate_photon_interact_with_object(
        struct mli_PhotonPropagation *env,
        const struct mli_IntersectionSurfaceNormal *isec);
chk_rc mli_propagate_photon_fresnel_refraction_and_reflection(
        struct mli_PhotonPropagation *env,
        const struct mli_IntersectionSurfaceNormal *isec);
chk_rc mli_propagate_photon_probability_passing_medium_coming_from(
        const struct mli_Scenery *scenery,
        const struct mli_Photon *photon,
        const struct mli_IntersectionSurfaceNormal *isec,
        double *probability_passing);
chk_rc mli_propagate_photon_pass_boundary_layer(
        struct mli_PhotonPropagation *env,
        const struct mli_IntersectionSurfaceNormal *isec,
        const struct mli_Fresnel fresnel);
struct mli_PhotonInteraction mliPhotonInteraction_from_Intersection(
        const int64_t type,
        const struct mli_Scenery *scenery,
        const struct mli_IntersectionSurfaceNormal *isec);
chk_rc mli_propagate_photon_env(struct mli_PhotonPropagation *env);
#endif
