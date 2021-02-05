/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliSceneryResources_read.h"
#include "mliMagicId.h"

int mliSceneryResources_read_capacity_from_file(
        struct mliSceneryResourcesCapacity *capacity,
        FILE *f)
{
        struct mliMagicId magic;

        /* magic identifier */
        mli_fread(&magic, sizeof(struct mliMagicId), 1u, f);
        mli_c(mliMagicId_has_word(&magic, "mliScnResCap"));
        mliMagicId_warn_version(&magic);

        /* payload */
        mli_fread(&capacity->num_objects, sizeof(uint32_t), 1u, f);
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
        struct mliMagicId magic;

        /* magic identifier */
        mli_fread(&magic, sizeof(struct mliMagicId), 1u, f);
        mli_c(mliMagicId_has_word(&magic, "mliSceneryResources"));
        mliMagicId_warn_version(&magic);

        /* payload */
        for (i = 0; i < res->num_objects; i++) {
                mli_c(mliObject_malloc_fread(&res->objects[i], f));
        }
        for (i = 0; i < res->num_functions; i++) {
                mli_c(mliFunc_malloc_fread(&res->functions[i], f));
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
