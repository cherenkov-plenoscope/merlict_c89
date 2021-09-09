/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery_serialize.h"
#include <math.h>
#include <stdint.h>
#include "mli_debug.h"

int mliScenery_fwrite(const struct mliScenery *scenery, FILE *f)
{
        struct mliMagicId magic;
        mli_check(mliMagicId_set(&magic, "mliScenery"));
        mli_check_fwrite(&magic, sizeof(struct mliMagicId), 1u, f);

        mli_check(mliGeometry_fwrite(&scenery->geometry, f));
        mli_check(mliAccelerator_fwrite(&scenery->accelerator, f));
        mli_check(mliMaterials_fwrite(&scenery->materials, f));
        mli_check(mliGeometryToMaterialMap_fwrite(&scenery->geomap, f));
        return 1;
error:
        return 0;
}

int mliScenery_malloc_fread(struct mliScenery *scenery, FILE *f)
{
        struct mliMagicId magic;

        mliScenery_free(scenery);

        mli_check_fread(&magic, sizeof(struct mliMagicId), 1u, f);
        mli_check(mliMagicId_has_word(&magic, "mliScenery"));
        mliMagicId_warn_version(&magic);

        mli_check(mliGeometry_malloc_fread(&scenery->geometry, f));
        mli_check(mliAccelerator_malloc_fread(&scenery->accelerator, f));
        mli_check(mliMaterials_malloc_fread(&scenery->materials, f));
        mli_check(mliGeometryToMaterialMap_malloc_fread(&scenery->geomap, f));
        return 1;
error:
        mliScenery_free(scenery);
        return 0;
}

int mliScenery_malloc_from_path(struct mliScenery *scenery, const char *path)
{
        FILE *f;
        f = fopen(path, "r");
        mli_check_message(f != NULL, "Can not open file for reading.");
        mli_check_message(mliScenery_malloc_fread(scenery, f), "Can not read file.");
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
        mli_check_message(f != NULL, "Can not open file for writing.");
        mli_check_message(mliScenery_fwrite(scenery, f), "Failed to write to file.");
        fclose(f);
        return 1;
error:
        if (f != NULL) {
                fclose(f);
        }
        return 0;
}
