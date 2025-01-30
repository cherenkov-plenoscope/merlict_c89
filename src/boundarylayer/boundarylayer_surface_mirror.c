/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "boundarylayer_surface_mirror.h"

int mli_BoundaryLayer_Surface_Mirror_equal(
        const struct mli_BoundaryLayer_Surface_Mirror *a,
        const struct mli_BoundaryLayer_Surface_Mirror *b)
{
        if (a->reflection_spectrum != b->reflection_spectrum) {
                return 0;
        }
        return 1;
}

int mli_BoundaryLayer_Surface_Mirror_to_io(
        const struct mli_BoundaryLayer_Surface_Mirror *self,
        struct mli_IO *f)
{
        struct mli_MagicId magic = mli_MagicId_init();
        chk(mli_MagicId_set(&magic, "mli_BoundaryLayer_Surface_Mirror"));
        chk_IO_write(&magic, sizeof(struct mli_MagicId), 1u, f);

        chk_IO_write(&self->reflection_spectrum, sizeof(uint64_t), 1u, f);

        return 1;
chk_error:
        return 0;
}

int mli_BoundaryLayer_Surface_Mirror_from_io(
        struct mli_BoundaryLayer_Surface_Mirror *self,
        struct mli_IO *f)
{
        struct mli_MagicId magic;
        chk_IO_read(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk(mli_MagicId_has_word(&magic, "mli_BoundaryLayer_Surface_Mirror"));
        mli_MagicId_warn_version(&magic);

        chk_IO_read(&self->reflection_spectrum, sizeof(uint64_t), 1u, f);

        return 1;
chk_error:
        return 0;
}
