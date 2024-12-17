/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliScenery_serialize.h"
#include <math.h>
#include <stdint.h>
#include "../chk/chk.h"
#include "../magicid/magicid.h"
#include "mliGeometry_serialize.h"
#include "mliAccelerator_serialize.h"
#include "../materials/materials_serialize.h"
#include "mliGeometryToMaterialMap_serialize.h"

int mliScenery_to_io(const struct mliScenery *scenery, struct mli_IO *f)
{
        struct mli_MagicId magic;
        chk(mli_MagicId_set(&magic, "mliScenery"));
        chk_IO_write(&magic, sizeof(struct mli_MagicId), 1u, f);

        chk(mliGeometry_to_io(&scenery->geometry, f));
        chk(mliAccelerator_to_io(&scenery->accelerator, f));
        chk(mli_Materials_to_io(&scenery->materials, f));
        chk(mliGeometryToMaterialMap_to_io(&scenery->geomap, f));
        return 1;
chk_error:
        return 0;
}

int mliScenery_from_io(struct mliScenery *scenery, struct mli_IO *f)
{
        struct mli_MagicId magic;

        mliScenery_free(scenery);

        chk_IO_read(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk(mli_MagicId_has_word(&magic, "mliScenery"));
        mli_MagicId_warn_version(&magic);

        chk(mliGeometry_from_io(&scenery->geometry, f));
        chk(mliAccelerator_from_io(&scenery->accelerator, f));
        chk(mli_Materials_from_io(&scenery->materials, f));
        chk(mliGeometryToMaterialMap_from_io(&scenery->geomap, f));
        return 1;
chk_error:
        mliScenery_free(scenery);
        return 0;
}

int mliScenery_malloc_from_path(struct mliScenery *scenery, const char *path)
{
        struct mli_IO f = mli_IO_init();
        chk_msg(mli_IO__open_file_cstr(&f, path, "r"), "Can't open file.");
        chk_msg(mliScenery_from_io(scenery, &f), "Can't read from file.");
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
        chk_msg(mliScenery_to_io(scenery, &f), "Can't write to file.");
        mli_IO_close(&f);
        return 1;
chk_error:
        mli_IO_close(&f);
        return 0;
}
