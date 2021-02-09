/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISCENERY_TAR_H_
#define MERLICT_C89_MLISCENERY_TAR_H_

#include "mliScenery.h"
#include "mliArchive.h"

int mliScenery_malloc_from_tar(
        struct mliScenery *scenery,
        const char *path);
int mliScenery_malloc_from_Archive(
        struct mliScenery *scenery,
        const struct mliArchive *archive);
#endif
