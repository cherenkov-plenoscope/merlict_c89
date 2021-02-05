/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISCENERYRESOURCES_SERIALIZE_H_
#define MERLICT_C89_MLISCENERYRESOURCES_SERIALIZE_H_

#include <stdio.h>
#include "mliSceneryResources.h"

int mliSceneryResources_fwrite(
        const struct mliSceneryResources *res,
        FILE *f);
int mliSceneryResources_malloc_fread(
        struct mliSceneryResources *res,
        FILE *f);
int mliSceneryResources_capacity_fwrite(
        const struct mliSceneryResources *res,
        FILE *f);
int mliSceneryResourcesCapacity_fread(
        struct mliSceneryResourcesCapacity *capacity,
        FILE *f);

#endif
