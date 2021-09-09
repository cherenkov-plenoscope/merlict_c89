/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery_serialize.h"
#include <math.h>
#include <stdint.h>
#include "mli_debug.h"

int mliScenery_fwrite(const struct mliScenery *scenery, FILE *f)
{
        struct mliMagicId magic;
        mli_c(mliMagicId_set(&magic, "mliScenery"));
        mli_check_fwrite(&magic, sizeof(struct mliMagicId), 1u, f);

        mli_c(mliGeometry_fwrite(&scenery->geometry, f));
        mli_c(mliAccelerator_fwrite(&scenery->accelerator, f));
        mli_c(mliMaterials_fwrite(&scenery->materials, f));
        mli_c(mliGeometryToMaterialMap_fwrite(&scenery->geomap, f));
        return 1;
error:
        return 0;
}

int mliScenery_malloc_fread(struct mliScenery *scenery, FILE *f)
{
        struct mliMagicId magic;

        mliScenery_free(scenery);

        mli_check_fread(&magic, sizeof(struct mliMagicId), 1u, f);
        mli_c(mliMagicId_has_word(&magic, "mliScenery"));
        mliMagicId_warn_version(&magic);

        mli_c(mliGeometry_malloc_fread(&scenery->geometry, f));
        mli_c(mliAccelerator_malloc_fread(&scenery->accelerator, f));
        mli_c(mliMaterials_malloc_fread(&scenery->materials, f));
        mli_c(mliGeometryToMaterialMap_malloc_fread(&scenery->geomap, f));
        return 1;
error:
        mliScenery_free(scenery);
        return 0;
}

int mliScenery_malloc_from_path(struct mliScenery *scenery, const char *path)
{
        FILE *f;
        f = fopen(path, "r");
        mli_check(f != NULL, "Can not open file for reading.");
        mli_check(mliScenery_malloc_fread(scenery, f), "Can not read file.");
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
        mli_check(f != NULL, "Can not open file for writing.");
        mli_check(mliScenery_fwrite(scenery, f), "Failed to write to file.");
        fclose(f);
        return 1;
error:
        if (f != NULL) {
                fclose(f);
        }
        return 0;
}
