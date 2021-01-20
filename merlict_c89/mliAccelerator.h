/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIACCELERATOR_H_
#define MERLICT_C89_MLIACCELERATOR_H_

#include <math.h>
#include <stdint.h>

#include "mli_debug.h"
#include "mliScenery.h"

struct mliAccelerator {
        struct mliSceneryResources resources;
        uint32_t num_robjects;
};

struct mliAccelerator mliAccelerator_init(void);

#endif
