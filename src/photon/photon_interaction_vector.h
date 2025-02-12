/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_PHOTON_INTERACTION_VECTOR_H_
#define MLI_PHOTON_INTERACTION_VECTOR_H_

#include "photon_interaction.h"
#include "../chk/chk.h"
#include "../geometry/geometry.h"
#include "../scenery/scenery.h"
#include "../vector/vector.h"

MLI_VECTOR_DEFINITON(mli_PhotonInteractionVector, struct mli_PhotonInteraction)

void mli_PhotonInteractionVector_print(
        const struct mli_PhotonInteractionVector *history,
        const struct mli_Scenery *scenery);

chk_rc mli_PhotonInteractionVector_time_of_flight(
        const struct mli_PhotonInteractionVector *history,
        const struct mli_Scenery *scenery,
        const double wavelength,
        double *total_time_of_flight);
#endif
