/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "scenery_serialize.h"
#include <math.h>
#include <stdint.h>
#include "../chk/chk.h"
#include "../magicid/magicid.h"
#include "../geometry/geometry_serialize.h"
#include "../accelerator/accelerator_serialize.h"
#include "../materials/materials_serialize.h"
#include "../geometrytomaterialmap/geometrytomaterialmap_serialize.h"

chk_rc mli_Scenery_to_io(const struct mli_Scenery *self, struct mli_IO *f)
{
        struct mli_MagicId magic;
        chk(mli_MagicId_set(&magic, "mli_Scenery"));
        chk_IO_write(&magic, sizeof(struct mli_MagicId), 1u, f);

        chk(mli_Geometry_to_io(&self->geometry, f));
        chk(mli_Accelerator_to_io(&self->accelerator, f));
        chk(mli_Materials_to_io(&self->materials, f));
        chk(mli_GeometryToMaterialMap_to_io(&self->geomap, f));
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Scenery_from_io(struct mli_Scenery *self, struct mli_IO *f)
{
        struct mli_MagicId magic;

        mli_Scenery_free(self);

        chk_IO_read(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk(mli_MagicId_has_word(&magic, "mli_Scenery"));
        mli_MagicId_warn_version(&magic);

        chk(mli_Geometry_from_io(&self->geometry, f));
        chk(mli_Accelerator_from_io(&self->accelerator, f));
        chk(mli_Materials_from_io(&self->materials, f));
        chk(mli_GeometryToMaterialMap_from_io(&self->geomap, f));
        return CHK_SUCCESS;
chk_error:
        mli_Scenery_free(self);
        return CHK_FAIL;
}

chk_rc mli_Scenery_malloc_from_path(struct mli_Scenery *self, const char *path)
{
        struct mli_IO f = mli_IO_init();
        chk_msg(mli_IO_open_file_cstr(&f, path, "r"), "Can't open file.");
        chk_msg(mli_Scenery_from_io(self, &f), "Can't read from file.");
        mli_IO_close(&f);
        return CHK_SUCCESS;
chk_error:
        mli_IO_close(&f);
        mli_Scenery_free(self);
        return CHK_FAIL;
}

chk_rc mli_Scenery_write_to_path(
        const struct mli_Scenery *self,
        const char *path)
{
        struct mli_IO f = mli_IO_init();
        chk_msg(mli_IO_open_file_cstr(&f, path, "w"), "Can't write to file.");
        chk_msg(mli_Scenery_to_io(self, &f), "Can't write to file.");
        mli_IO_close(&f);
        return CHK_SUCCESS;
chk_error:
        mli_IO_close(&f);
        return CHK_FAIL;
}
