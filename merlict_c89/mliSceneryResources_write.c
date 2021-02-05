/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliSceneryResources_write.h"
#include "mliMagicId.h"

int mliSceneryResources_capacity_fwrite(
        const struct mliSceneryResources *res,
        FILE *f)
{
        struct mliMagicId magic = mliMagicId_init();

        /* magic identifier */
        mli_c(mliMagicId_set(&magic, "mliScnResCap"));
        mli_fwrite(&magic, sizeof(struct mliMagicId), 1u, f);

        /* payload */
        mli_fwrite(&res->num_objects, sizeof(uint32_t), 1u, f);
        mli_fwrite(&res->num_functions, sizeof(uint32_t), 1u, f);
        mli_fwrite(&res->num_colors, sizeof(uint32_t), 1u, f);
        mli_fwrite(&res->num_media, sizeof(uint32_t), 1u, f);
        mli_fwrite(&res->num_surfaces, sizeof(uint32_t), 1u, f);
        return 1;
error:
        return 0;
}

int mliSceneryResources_fwrite(
        const struct mliSceneryResources *res,
        FILE *f)
{
        uint64_t i;
        struct mliMagicId magic = mliMagicId_init();

        /* magic identifier */
        mli_c(mliMagicId_set(&magic, "mliSceneryResources"));
        mli_fwrite(&magic, sizeof(struct mliMagicId), 1u, f);

        for (i = 0; i < res->num_objects; i++) {
                mliObject_fwrite(&res->objects[i], f);
        }

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
