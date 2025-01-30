/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "boundarylayer_medium_transparent.h"

int mli_BoundaryLayer_Medium_Transparent_equal(
        const struct mli_BoundaryLayerMedium_Transparent *a,
        const struct mli_BoundaryLayerMedium_Transparent *b)
{
        if (a->refraction_spectrum != b->refraction_spectrum) {
                return 0;
        }
        return 1;
}

int mli_BoundaryLayer_Medium_Transparent_to_io(
        const struct mli_BoundaryLayerMedium_Transparent *self,
        struct mli_IO *f)
{
        struct mli_MagicId magic = mli_MagicId_init();
        chk(mli_MagicId_set(&magic, "mli_BoundaryLayerMedium_Transparent"));
        chk_IO_write(&magic, sizeof(struct mli_MagicId), 1u, f);

        chk_IO_write(&self->refraction_spectrum, sizeof(uint64_t), 1u, f);

        return 1;
chk_error:
        return 0;
}

int mli_BoundaryLayer_Medium_Transparent_from_io(
        struct mli_BoundaryLayerMedium_Transparent *self,
        struct mli_IO *f)
{
        struct mli_MagicId magic;
        chk_IO_read(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk(mli_MagicId_has_word(
                &magic, "mli_BoundaryLayerMedium_Transparent"));
        mli_MagicId_warn_version(&magic);

        chk_IO_read(&self->refraction_spectrum, sizeof(uint64_t), 1u, f);

        return 1;
chk_error:
        return 0;
}
