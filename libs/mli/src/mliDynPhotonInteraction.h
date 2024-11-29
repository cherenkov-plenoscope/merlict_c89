/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIDYNPHOTONINTERACTION_H_
#define MLIDYNPHOTONINTERACTION_H_

#include "mliPhotonInteraction.h"
#include "mliGeometry.h"
#include "mliScenery.h"
#include "../../mtl/src/vector.h"

MTL_VECTOR_DEFINITON(mliDynPhotonInteraction, struct mliPhotonInteraction)

void mliDynPhotonInteraction_print(
        const struct mliDynPhotonInteraction *history,
        const struct mliScenery *scenery);

int mliDynPhotonInteraction_time_of_flight(
        const struct mliDynPhotonInteraction *history,
        const struct mliScenery *scenery,
        const double wavelength,
        double *total_time_of_flight);
#endif
