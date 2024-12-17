/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_SCENERY_TAR_H_
#define MLI_SCENERY_TAR_H_

#include "scenery.h"
#include "../archive/archive.h"

int mliScenery_from_io_tar(struct mliScenery *scenery, FILE *f);
int mliScenery_malloc_from_path_tar(
        struct mliScenery *scenery,
        const char *path);
int mliScenery_malloc_from_Archive(
        struct mliScenery *scenery,
        const struct mli_Archive *archive);
#endif
