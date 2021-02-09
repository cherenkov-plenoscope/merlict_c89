/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLITRACER_H_
#define MERLICT_C89_MLITRACER_H_

#include "mliScenery.h"

struct mliColor mli_trace(
        const struct mliScenery *combine,
        const struct mliRay ray);
#endif
