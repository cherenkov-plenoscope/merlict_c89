/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliSceneryResources.h"
#include "mli_debug.h"

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
        res.default_medium = 0u;

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
        const struct mliSceneryResourcesCapacity rescap)
{
        uint64_t i;
        mliSceneryResources_free(res);
        res->num_functions = rescap.num_functions;
        res->num_colors = rescap.num_colors;
        res->num_surfaces = rescap.num_surfaces;
        res->num_media = rescap.num_media;

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

int mliSceneryResources_cpy(
        struct mliSceneryResources *destination,
        const struct mliSceneryResources *source)
{
        uint64_t i;
        mli_check(
                destination->num_functions == source->num_functions,
                "Expected source and destination to have same "
                "num_functions.");
        mli_check(
                destination->num_colors == source->num_colors,
                "Expected source and destination to have same "
                "num_colors.");
        mli_check(
                destination->num_surfaces == source->num_surfaces,
                "Expected source and destination to have same "
                "num_surfaces.");
        mli_check(
                destination->num_media == source->num_media,
                "Expected source and destination to have same "
                "num_media.");

        /* default_medium */
        destination->default_medium = source->default_medium;

        /* copy surfaces */
        for (i = 0; i < source->num_functions; i++) {
                mli_check(
                        mliFunc_malloc(
                                &destination->functions[i],
                                source->functions[i].num_points),
                        "Failed to allocate struct mliFunc in Resources.");
                mli_check(
                        mliFunc_cpy(
                                &destination->functions[i],
                                &source->functions[i]),
                        "Failed to copy function to Resources.");
        }
        for (i = 0; i < source->num_colors; i++) {
                destination->colors[i] = source->colors[i];
        }
        for (i = 0; i < source->num_media; i++) {
                destination->media[i] = source->media[i];
        }
        for (i = 0; i < source->num_surfaces; i++) {
                destination->surfaces[i] = source->surfaces[i];
        }

        return 1;
error:
        return 0;
}

void mliSceneryResources_info_fprint(FILE *f, const struct mliSceneryResources *res)
{
        fprintf(f, "__mliSceneryResources__\n");
        fprintf(f, "- default_medium: %d\n", res->default_medium);
}
