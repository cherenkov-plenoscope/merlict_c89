/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliSceneryResources.h"

/* Copyright 2019 Sebastian Achim Mueller */

struct mliSceneryResourcesCapacity mliSceneryResourcesCapacity_init(void)
{
        struct mliSceneryResourcesCapacity cap;
        cap.num_functions = 0;
        cap.num_colors = 0;
        cap.num_surfaces = 0;
        cap.num_media = 0;
        return cap;
}

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
        uint64_t i;
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
        uint64_t i;
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
