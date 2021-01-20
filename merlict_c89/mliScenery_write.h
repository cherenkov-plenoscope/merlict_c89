/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISCENERY_WRITE_H_
#define MERLICT_C89_MLISCENERY_WRITE_H_

#include <math.h>
#include <stdint.h>

#include "mli_debug.h"
#include "mliScenery.h"
#include "mliSceneryResources_write.h"

int mliScenery_write_to_path(
        const struct mliScenery *scenery,
        const char *path);
int mliScenery_write_to_file(const struct mliScenery *scenery, FILE *f);

#endif
