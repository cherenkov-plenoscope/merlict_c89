/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery_serialize.h"
#include "mliMaterials_serialize.h"
#include "mliMagicId.h"

int mliScenery_malloc_fread(struct mliScenery *scenery, FILE *f)
{
        uint32_t i;
        uint32_t num_objects = 0u;
        uint32_t num_robjects = 0u;
        struct mliMagicId magic;

        /* magic identifier */
        mli_fread(&magic, sizeof(struct mliMagicId), 1u, f);
        mli_c(mliMagicId_has_word(&magic, "mliScenery"));
        mliMagicId_warn_version(&magic);

        /* payload */
        mli_fread(&num_objects, sizeof(uint32_t), 1u, f);
        mli_fread(&num_robjects, sizeof(uint32_t), 1u, f);

        mli_check(
                mliScenery_malloc(scenery, num_objects, num_robjects),
                "Failed to malloc robjects in mliScenery.");

        for (i = 0; i < scenery->num_objects; i++) {
                mli_check(
                        mliObject_malloc_fread(&scenery->objects[i], f),
                        "Failed to read object into scenery.");
        }

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

int mliScenery_fwrite(const struct mliScenery *scenery, FILE *f)
{
        uint32_t i;
        struct mliMagicId magic;

        /* magic identifier */
        mli_c(mliMagicId_set(&magic, "mliScenery"));
        mli_fwrite(&magic, sizeof(struct mliMagicId), 1u, f);

        /* payload */
        mli_write_type(uint32_t, scenery->num_objects, f);
        mli_write_type(uint32_t, scenery->num_robjects, f);

        for (i = 0; i < scenery->num_objects; i++) {
                mli_check(mliObject_fwrite(&scenery->objects[i], f),
                        "Failed to write objects.");
        }
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
