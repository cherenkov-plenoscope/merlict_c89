/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISCENERY_READ_H_
#define MERLICT_C89_MLISCENERY_READ_H_

#include <math.h>
#include <stdint.h>

#include "mli_debug.h"
#include "mliScenery.h"
#include "mliScenery_write.h"
#include "mliSceneryResources_read.h"

int mliScenery_malloc_from_tape_archive(
        struct mliScenery *scenery,
        const char *path);
int mliScenery_malloc_from_path(struct mliScenery *scenery, const char *path);
int mliScenery_malloc_fread(struct mliScenery *scenery, FILE *f);
#endif
