/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLISCENERYRESOURCES_WRITE_H_
#define MERLICT_MLISCENERYRESOURCES_WRITE_H_

#include <math.h>
#include <stdint.h>
#include "mli_debug.h"
#include "mliSceneryResources.h"

int mliSceneryResources_write_capacity_to_file(
        const struct mliSceneryResources *res,
        FILE *f)
{
        mli_fwrite(&res->num_functions, sizeof(uint32_t), 1u, f);
        mli_fwrite(&res->num_colors, sizeof(uint32_t), 1u, f);
        mli_fwrite(&res->num_media, sizeof(uint32_t), 1u, f);
        mli_fwrite(&res->num_surfaces, sizeof(uint32_t), 1u, f);
        return 1;
error:
        return 0;
}

int mliSceneryResources_append_to_file(
        const struct mliSceneryResources *res,
        FILE *f)
{
        size_t i;
        for (i = 0; i < res->num_functions; i++) {
                mliFunc_fwrite(&res->functions[i], f);
        }
        mli_fwrite(res->colors, sizeof(struct mliColor), res->num_colors, f);
        mli_fwrite(res->media, sizeof(struct mliMedium), res->num_media, f);
        mli_fwrite(
                res->surfaces, sizeof(struct mliSurface), res->num_surfaces, f);
        return 1;
error:
        return 0;
}

#endif
