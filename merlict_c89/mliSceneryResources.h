/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISCENERYRESOURCES_H_
#define MERLICT_C89_MLISCENERYRESOURCES_H_

#include <math.h>
#include <stdint.h>

#include "mli_debug.h"
#include "mliObject.h"
#include "mliColor.h"
#include "mliFunc.h"
#include "mliSurface.h"
#include "mliMedium.h"

struct mliSceneryResourcesCapacity {
        uint32_t num_objects;
        uint32_t num_functions;
        uint32_t num_colors;
        uint32_t num_surfaces;
        uint32_t num_media;
};

struct mliSceneryResourcesCapacity mliSceneryResourcesCapacity_init(void);

struct mliSceneryResources {
        uint32_t num_objects;
        struct mliObject *objects;

        uint32_t num_functions;
        struct mliFunc *functions;

        uint32_t num_colors;
        struct mliColor *colors;

        uint32_t num_surfaces;
        struct mliSurface *surfaces;

        uint32_t num_media;
        struct mliMedium *media;

        uint32_t default_medium;
};

int mliSceneryResources_malloc(
        struct mliSceneryResources *res,
        const struct mliSceneryResourcesCapacity rescap);
void mliSceneryResources_free(struct mliSceneryResources *res);
struct mliSceneryResources mliSceneryResources_init(void);
int mliSceneryResources_cpy(
        struct mliSceneryResources *destination,
        const struct mliSceneryResources *source);
void mliSceneryResources_fprint(FILE *f, const struct mliSceneryResources *res);
#endif
