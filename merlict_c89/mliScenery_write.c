/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery_write.h"
#include "mliMagicId.h"

int mliScenery_write_to_path(const struct mliScenery *scenery, const char *path)
{
        FILE *f;
        f = fopen(path, "w");
        mli_check(f != NULL, "Can not open Scenery-file for writing.");
        mli_check(
                mliScenery_fwrite(scenery, f),
                "Failed to write to file.");
        fclose(f);
        return 1;
error:
        if (f != NULL) {
                fclose(f);
        }
        return 0;
}

int mliScenery_fwrite(const struct mliScenery *scenery, FILE *f)
{
        struct mliMagicId magic;

        /* magic identifier */
        mli_c(mliMagicId_set(&magic, "mliScenery"));
        mli_fwrite(&magic, sizeof(struct mliMagicId), 1u, f);

        /* payload */
        mli_write_type(uint32_t, scenery->num_robjects, f);
        mli_check(
                mliSceneryResources_capacity_fwrite(
                        &scenery->resources, f),
                "Failed to write SceneryResources' capacity to file.");
        mli_check(
                mliSceneryResources_fwrite(&scenery->resources, f),
                "Can not write SceneryResources to file.");
        mli_fwrite(
                scenery->robjects, sizeof(uint32_t), scenery->num_robjects, f);
        mli_fwrite(
                scenery->robject_ids,
                sizeof(uint32_t),
                scenery->num_robjects,
                f);
        mli_fwrite(
                scenery->robject_boundary_layers,
                sizeof(struct mliBoundaryLayer),
                scenery->num_robjects,
                f);
        mli_fwrite(
                scenery->robject2root,
                sizeof(struct mliHomTraComp),
                scenery->num_robjects,
                f);

        return 1;
error:
        return 0;
}
