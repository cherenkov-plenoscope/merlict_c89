/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLISCENERY_TAR_H_
#define MLISCENERY_TAR_H_

#include "mliScenery.h"
#include "mliArchive.h"

int mliScenery_malloc_fread_tar(struct mliScenery *scenery, FILE *f);
int mliScenery_malloc_from_path_tar(
        struct mliScenery *scenery,
        const char *path);
int mliScenery_malloc_from_Archive(
        struct mliScenery *scenery,
        const struct mliArchive *archive);
#endif
