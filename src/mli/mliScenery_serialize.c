/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery_serialize.h"
#include <math.h>
#include <stdint.h>
#include "../chk/chk.h"
#include "mliMagicId.h"
#include "mliGeometry_serialize.h"
#include "mliAccelerator_serialize.h"
#include "mliMaterials_serialize.h"
#include "mliGeometryToMaterialMap_serialize.h"

int mliScenery_fwrite(const struct mliScenery *scenery, struct mli_IO *f)
{
        struct mliMagicId magic;
        chk(mliMagicId_set(&magic, "mliScenery"));
        chk_IO_write(&magic, sizeof(struct mliMagicId), 1u, f);

        chk(mliGeometry_fwrite(&scenery->geometry, f));
        chk(mliAccelerator_fwrite(&scenery->accelerator, f));
        chk(mliMaterials_fwrite(&scenery->materials, f));
        chk(mliGeometryToMaterialMap_fwrite(&scenery->geomap, f));
        return 1;
chk_error:
        return 0;
}

int mliScenery_malloc_fread(struct mliScenery *scenery, struct mli_IO *f)
{
        struct mliMagicId magic;

        mliScenery_free(scenery);

        chk_IO_read(&magic, sizeof(struct mliMagicId), 1u, f);
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
        struct mli_IO f = mli_IO_init();
        chk_msg(mli_IO__open_file_cstr(&f, path, "r"), "Can't open file.");
        chk_msg(mliScenery_malloc_fread(scenery, &f), "Can't read from file.");
        mli_IO_close(&f);
        return 1;
chk_error:
        mli_IO_close(&f);
        mliScenery_free(scenery);
        return 0;
}

int mliScenery_write_to_path(const struct mliScenery *scenery, const char *path)
{
        struct mli_IO f = mli_IO_init();
        chk_msg(mli_IO__open_file_cstr(&f, path, "w"), "Can't write to file.");
        chk_msg(mliScenery_fwrite(scenery, &f), "Can't write to file.");
        mli_IO_close(&f);
        return 1;
chk_error:
        mli_IO_close(&f);
        return 0;
}
