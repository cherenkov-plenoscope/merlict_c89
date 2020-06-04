/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLISCENERYRESOURCES_READ_H_
#define MERLICT_MLISCENERYRESOURCES_READ_H_

#include <math.h>
#include <stdint.h>
#include "mli_debug.h"
#include "mliSceneryResources.h"

int mliSceneryResources_read_capacity_from_file(
        struct mliSceneryResourcesCapacity *capacity,
        FILE *f)
{
        mli_fread(&capacity->num_functions, sizeof(uint32_t), 1u, f);
        mli_fread(&capacity->num_colors, sizeof(uint32_t), 1u, f);
        mli_fread(&capacity->num_media, sizeof(uint32_t), 1u, f);
        mli_fread(&capacity->num_surfaces, sizeof(uint32_t), 1u, f);
        return 1;
error:
        return 0;
}

int mliSceneryResources_read_from_file(struct mliSceneryResources *res, FILE *f)
{
        uint64_t i;
        for (i = 0; i < res->num_functions; i++) {
                mli_c(mliFunc_malloc_from_file(&res->functions[i], f));
        }
        mli_fread(res->colors, sizeof(struct mliColor), res->num_colors, f);
        mli_fread(res->media, sizeof(struct mliMedium), res->num_media, f);
        mli_fread(
                res->surfaces,
                sizeof(struct mliBoundaryLayer),
                res->num_surfaces,
                f);
        return 1;
error:
        return 0;
}

#endif
