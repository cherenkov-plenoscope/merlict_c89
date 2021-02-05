/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery_read.h"
#include "mliSceneryResources.h"

int mliScenery_malloc_from_tape_archive(
        struct mliScenery *scenery,
        const char *path)
{
        struct mliUserScenery user_scenery = mliUserScenery_init();
        mli_check(
                mliUserScenery_malloc_from_tape_archive(&user_scenery, path),
                "Failed to malloc mliUserScenery from tape-archive.");
        mli_check(
                mliScenery_malloc_from_mliUserScenery(scenery, &user_scenery),
                "Failed to malloc mliScenery from mliUserScenery.");
        return 1;
error:
        return 0;
}

int mliScenery_malloc_from_path(struct mliScenery *scenery, const char *path)
{
        FILE *f;
        f = fopen(path, "r");
        mli_check(f != NULL, "Can not open Scenery-file for reading.");
        mli_check(
                mliScenery_malloc_fread(scenery, f),
                "Can not read scenery-file.");
        fclose(f);
        return 1;
error:
        if (f != NULL) {
                fclose(f);
        }
        mliScenery_free(scenery);
        return 0;
}

int mliScenery_malloc_fread(struct mliScenery *scenery, FILE *f)
{
        uint32_t num_robjects = 0u;
        struct mliMagicId magic;
        struct mliSceneryResourcesCapacity rescap =
                mliSceneryResourcesCapacity_init();

        /* magic identifier */
        mli_fread(&magic, sizeof(struct mliMagicId), 1u, f);
        mli_c(mliMagicId_has_word(&magic, "mliScenery"));
        mliMagicId_warn_version(&magic);

        /* payload */
        mli_fread(&num_robjects, sizeof(uint32_t), 1u, f);

        mli_check(
                mliScenery_malloc(scenery, num_robjects),
                "Failed to malloc robjects in mliScenery.");

        mli_check(
                mliSceneryResourcesCapacity_fread(&rescap, f),
                "Failed to read SceneryResources' capacity from file");

        mli_check(
                mliSceneryResources_malloc(&scenery->resources, rescap),
                "Failed to malloc SceneryResources.");

        mli_check(
                mliSceneryResources_malloc_fread(&scenery->resources, f),
                "Failed to read SceneryResources from file.");

        mli_fread(
                scenery->robjects, sizeof(uint32_t), scenery->num_robjects, f);
        mli_fread(
                scenery->robject_ids,
                sizeof(uint32_t),
                scenery->num_robjects,
                f);
        mli_fread(
                scenery->robject_boundary_layers,
                sizeof(struct mliBoundaryLayer),
                scenery->num_robjects,
                f);
        mli_fread(
                scenery->robject2root,
                sizeof(struct mliHomTraComp),
                scenery->num_robjects,
                f);

        return 1;
error:
        return 0;
}
