/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliSurface_serialize.h"
#include "mliMagicId.h"
#include "../func/func_serialize.h"

int mliSurface_to_io(const struct mliSurface *srf, struct mli_IO *f)
{
        struct mliMagicId magic = mliMagicId_init();
        /* magic identifier */
        chk(mliMagicId_set(&magic, "mliSurface"));
        chk_IO_write(&magic, sizeof(struct mliMagicId), 1u, f);

        chk_IO_write(&srf->material, sizeof(uint32_t), 1u, f);
        chk(mli_Func_to_io(&srf->specular_reflection, f));
        chk(mli_Func_to_io(&srf->diffuse_reflection, f));

        return 1;
chk_error:
        return 0;
}

int mliSurface_from_io(struct mliSurface *srf, struct mli_IO *f)
{
        struct mliMagicId magic;
        /* magic identifier */
        chk_IO_read(&magic, sizeof(struct mliMagicId), 1u, f);
        chk_msg(mliMagicId_has_word(&magic, "mliSurface"),
                "Expected MagicID 'mliSurface'.");
        mliMagicId_warn_version(&magic);

        chk_IO_read(&srf->material, sizeof(uint32_t), 1u, f);
        chk(mli_Func_from_io(&srf->specular_reflection, f));
        chk(mli_Func_from_io(&srf->diffuse_reflection, f));

        return 1;
chk_error:
        return 0;
}
