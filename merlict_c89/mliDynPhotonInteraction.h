/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIDYNPHOTONINTERACTION_H_
#define MERLICT_C89_MLIDYNPHOTONINTERACTION_H_

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>

#include "mli_debug.h"
#include "mliPhotonInteraction.h"
#include "mliScenery.h"

MLIDYNARRAY_DEFINITON(mli, PhotonInteraction, struct mliPhotonInteraction)

void mliDynPhotonInteraction_print(
        const struct mliDynPhotonInteraction *history,
        const struct mliScenery *scenery);
#endif
