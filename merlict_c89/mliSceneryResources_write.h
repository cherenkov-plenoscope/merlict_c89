/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISCENERYRESOURCES_WRITE_H_
#define MERLICT_C89_MLISCENERYRESOURCES_WRITE_H_

#include <math.h>
#include <stdint.h>

#include "mli_debug.h"
#include "mliSceneryResources.h"

int mliSceneryResources_append_to_file(
        const struct mliSceneryResources *res,
        FILE *f);
int mliSceneryResources_write_capacity_to_file(
        const struct mliSceneryResources *res,
        FILE *f);
#endif
