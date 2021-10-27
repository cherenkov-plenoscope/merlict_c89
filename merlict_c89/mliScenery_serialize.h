/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLISCENERY_SERIALIZE_H_
#define MLISCENERY_SERIALIZE_H_

#include "mliScenery.h"

int mliScenery_fwrite(const struct mliScenery *scenery, FILE *f);
int mliScenery_malloc_fread(struct mliScenery *scenery, FILE *f);

int mliScenery_malloc_from_path(struct mliScenery *scenery, const char *path);
int mliScenery_write_to_path(
        const struct mliScenery *scenery,
        const char *path);
#endif
