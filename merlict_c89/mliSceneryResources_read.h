/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISCENERYRESOURCES_READ_H_
#define MERLICT_C89_MLISCENERYRESOURCES_READ_H_

#include <math.h>
#include <stdint.h>

#include "mli_debug.h"
#include "mliSceneryResources.h"

int mliSceneryResources_read_from_file(
        struct mliSceneryResources *res,
        FILE *f);
int mliSceneryResources_read_capacity_from_file(
        struct mliSceneryResourcesCapacity *capacity,
        FILE *f);
#endif
