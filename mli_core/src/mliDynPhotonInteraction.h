/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIDYNPHOTONINTERACTION_H_
#define MLIDYNPHOTONINTERACTION_H_

#include "mliPhotonInteraction.h"
#include "mliGeometry.h"

MLIDYNARRAY_DEFINITON(mli, PhotonInteraction, struct mliPhotonInteraction)

void mliDynPhotonInteraction_print(
        const struct mliDynPhotonInteraction *history,
        const struct mliGeometry *scenery);
#endif
