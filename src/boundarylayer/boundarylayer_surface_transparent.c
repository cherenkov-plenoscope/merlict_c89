/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "boundarylayer_surface_transparent.h"
#include "../magicid/magicid.h"
#include "../io/io.h"

int mli_BoundaryLayer_Surface_Transparent_equal(
        const struct mli_BoundaryLayer_Surface_Transparent *a,
        const struct mli_BoundaryLayer_Surface_Transparent *b)
{
        if (a->nothing != b->nothing) {
                return 0;
        }
        return 1;
}

int mli_BoundaryLayer_Surface_Transparent_to_io(
        const struct mli_BoundaryLayer_Surface_Transparent *self,
        struct mli_IO *f)
{
        struct mli_MagicId magic = mli_MagicId_init();
        chk(mli_MagicId_set(&magic, "mli_BoundaryLayer_Surface_Transparent"));
        chk_IO_write(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk_IO_write(&self->nothing, sizeof(uint64_t), 1u, f);
        return 1;
chk_error:
        return 0;
}

int mli_BoundaryLayer_Surface_Transparent_from_io(
        struct mli_BoundaryLayer_Surface_Transparent *self,
        struct mli_IO *f)
{
        struct mli_MagicId magic;
        chk_IO_read(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk(mli_MagicId_has_word(
                &magic, "mli_BoundaryLayer_Surface_Transparent"));
        mli_MagicId_warn_version(&magic);
        chk_IO_read(&self->nothing, sizeof(uint64_t), 1u, f);
        return 1;
chk_error:
        return 0;
}

int mli_BoundaryLayer_Surface_Transparent_from_json_string(
        struct mli_BoundaryLayer_Surface_Transparent *self,
        const struct mli_Map *spectra_names,
        const struct mli_String *json_string)
{
        chk(spectra_names != NULL);
        chk(json_string != NULL);

        self->nothing = 0;

        return 1;
chk_error:
        return 0;
}
