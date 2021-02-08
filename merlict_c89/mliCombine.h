/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLICOMBINE_H_
#define MERLICT_C89_MLICOMBINE_H_

#include <math.h>
#include <stdint.h>

#include "mli_debug.h"
#include "mliScenery.h"
#include "mliAccelerator.h"

struct mliCombine {
        const struct mliScenery *scenery;
        struct mliAccelerator *accelerator;
};

#endif
