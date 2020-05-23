/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLISCENERYRESOURCES_H_
#define MERLICT_MLISCENERYRESOURCES_H_

#include <math.h>
#include <stdint.h>
#include "mli_debug.h"
#include "mliColor.h"
#include "mliFunc.h"
#include "mliSurface.h"
#include "mliMedium.h"

struct mliSceneryResourcesCapacity {
        uint32_t num_functions;
        uint32_t num_colors;
        uint32_t num_surfaces;
        uint32_t num_media;
};

struct mliSceneryResourcesCapacity mliSceneryResourcesCapacity_init(void)
{
        struct mliSceneryResourcesCapacity cap;
        cap.num_functions = 0;
        cap.num_colors = 0;
        cap.num_surfaces = 0;
        cap.num_media = 0;
        return cap;
}

struct mliSceneryResources {
        uint32_t num_functions;
        struct mliFunc *functions;

        uint32_t num_colors;
        struct mliColor *colors;

        uint32_t num_surfaces;
        struct mliSurface *surfaces;

        uint32_t num_media;
        struct mliMedium *media;
};

struct mliSceneryResources mliSceneryResources_init(void)
{
        struct mliSceneryResources res;
        res.num_functions = 0u;
        res.functions = NULL;

        res.num_colors = 0u;
        res.colors = NULL;

        res.num_media = 0u;
        res.media = NULL;

        res.num_surfaces = 0u;
        res.surfaces = NULL;
        return res;
}

void mliSceneryResources_free(struct mliSceneryResources *res)
{
        size_t i;
        for (i = 0; i < res->num_functions; i++) {
                mliFunc_free(&(res->functions[i]));
        }
        free(res->functions);
        free(res->colors);
        free(res->media);
        free(res->surfaces);
        (*res) = mliSceneryResources_init();
}

int mliSceneryResources_malloc(
        struct mliSceneryResources *res,
        const struct mliSceneryResourcesCapacity capacity)
{
        size_t i;
        mliSceneryResources_free(res);
        res->num_functions = capacity.num_functions;
        res->num_colors = capacity.num_colors;
        res->num_surfaces = capacity.num_surfaces;
        res->num_media = capacity.num_media;

        mli_malloc(res->functions, struct mliFunc, res->num_functions);
        for (i = 0; i < res->num_functions; i++) {
                res->functions[i] = mliFunc_init();
        }
        mli_malloc(res->colors, struct mliColor, res->num_colors);
        mli_malloc(res->media, struct mliMedium, res->num_media);
        mli_malloc(res->surfaces, struct mliSurface, res->num_surfaces);
        return 1;
error:
        mliSceneryResources_free(res);
        return 0;
}

#endif
