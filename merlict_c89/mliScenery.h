/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISCENERY_H_
#define MERLICT_C89_MLISCENERY_H_

#include "mliGeometry.h"
#include "mliMaterials.h"
#include "mliAccelerator.h"
#include "mliArchive.h"

struct mliScenery {
        struct mliGeometry geometry;
        struct mliAccelerator accelerator;
        struct mliMaterials materials;
};

struct mliScenery mliScenery_init(void);

void mliScenery_free(struct mliScenery *scenery);


int mliScenery_malloc_from_tar(
        struct mliScenery *scenery,
        const char *path);
int mliScenery_malloc_from_Archive(
        struct mliScenery *scenery,
        const struct mliArchive *archive);

#endif
