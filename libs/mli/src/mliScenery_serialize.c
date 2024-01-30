/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery_serialize.h"
#include <math.h>
#include <stdint.h>
#include "chk.h"
#include "mliMagicId.h"
#include "mliGeometry_serialize.h"
#include "mliAccelerator_serialize.h"
#include "mliMaterials_serialize.h"
#include "mliGeometryToMaterialMap_serialize.h"


int mliScenery_fwrite(const struct mliScenery *scenery, FILE *f)
{
        struct mliMagicId magic;
        chk(mliMagicId_set(&magic, "mliScenery"));
        chk_fwrite(&magic, sizeof(struct mliMagicId), 1u, f);

        chk(mliGeometry_fwrite(&scenery->geometry, f));
        chk(mliAccelerator_fwrite(&scenery->accelerator, f));
        chk(mliMaterials_fwrite(&scenery->materials, f));
        chk(mliGeometryToMaterialMap_fwrite(&scenery->geomap, f));
        return 1;
chk_error:
        return 0;
}

int mliScenery_malloc_fread(struct mliScenery *scenery, FILE *f)
{
        struct mliMagicId magic;

        mliScenery_free(scenery);

        chk_fread(&magic, sizeof(struct mliMagicId), 1u, f);
        chk(mliMagicId_has_word(&magic, "mliScenery"));
        mliMagicId_warn_version(&magic);

        chk(mliGeometry_malloc_fread(&scenery->geometry, f));
        chk(mliAccelerator_malloc_fread(&scenery->accelerator, f));
        chk(mliMaterials_malloc_fread(&scenery->materials, f));
        chk(mliGeometryToMaterialMap_malloc_fread(&scenery->geomap, f));
        return 1;
chk_error:
        mliScenery_free(scenery);
        return 0;
}

int mliScenery_malloc_from_path(struct mliScenery *scenery, const char *path)
{
        FILE *f;
        f = fopen(path, "r");
        chk_msg(f != NULL, "Can not open file for reading.");
        chk_msg(mliScenery_malloc_fread(scenery, f), "Can not read file.");
        fclose(f);
        return 1;
chk_error:
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
        chk_msg(f != NULL, "Can not open file for writing.");
        chk_msg(mliScenery_fwrite(scenery, f), "Failed to write to file.");
        fclose(f);
        return 1;
chk_error:
        if (f != NULL) {
                fclose(f);
        }
        return 0;
}
