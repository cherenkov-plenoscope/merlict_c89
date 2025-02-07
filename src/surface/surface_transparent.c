/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "surface_transparent.h"
#include "../chk/chk.h"
#include "../magicid/magicid.h"
#include "../io/io.h"
#include "../materials/materials.h"

mli_bool mli_Surface_Transparent_equal(
        const struct mli_Surface_Transparent *a,
        const struct mli_Surface_Transparent *b)
{
        if (a->nothing != b->nothing) {
                return MLI_FALSE;
        }
        return MLI_TRUE;
}

chk_rc mli_Surface_Transparent_to_io(
        const struct mli_Surface_Transparent *self,
        struct mli_IO *f)
{
        struct mli_MagicId magic = mli_MagicId_init();
        chk(mli_MagicId_set(&magic, "mli_Surface_Transparent"));
        chk_IO_write(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk_IO_write(&self->nothing, sizeof(uint64_t), 1u, f);
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Surface_Transparent_from_io(
        struct mli_Surface_Transparent *self,
        struct mli_IO *f)
{
        struct mli_MagicId magic;
        chk_IO_read(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk(mli_MagicId_has_word(&magic, "mli_Surface_Transparent"));
        mli_MagicId_warn_version(&magic);
        chk_IO_read(&self->nothing, sizeof(uint64_t), 1u, f);
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Surface_Transparent_from_json_string(
        struct mli_Surface_Transparent *self,
        const struct mli_Map *spectra_names,
        const struct mli_String *json_string)
{
        chk(spectra_names != NULL);
        chk(json_string != NULL);

        self->nothing = 0;

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_Surface_Transparent_valid_wrt_materials(
        const struct mli_Surface_Transparent *self,
        const struct mli_Materials *materials)
{
        chk(self);
        chk(materials);
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}
