/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliSurface_serialize.h"
#include "../magicid/magicid.h"
#include "../func/func_serialize.h"

int mliSurface_to_io(const struct mliSurface *srf, struct mli_IO *f)
{
        struct mli_MagicId magic = mli_MagicId_init();
        /* magic identifier */
        chk(mli_MagicId_set(&magic, "mliSurface"));
        chk_IO_write(&magic, sizeof(struct mli_MagicId), 1u, f);

        chk_IO_write(&srf->material, sizeof(uint32_t), 1u, f);
        chk(mli_Func_to_io(&srf->specular_reflection, f));
        chk(mli_Func_to_io(&srf->diffuse_reflection, f));

        return 1;
chk_error:
        return 0;
}

int mliSurface_from_io(struct mliSurface *srf, struct mli_IO *f)
{
        struct mli_MagicId magic;
        /* magic identifier */
        chk_IO_read(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk_msg(mli_MagicId_has_word(&magic, "mliSurface"),
                "Expected MagicID 'mliSurface'.");
        mli_MagicId_warn_version(&magic);

        chk_IO_read(&srf->material, sizeof(uint32_t), 1u, f);
        chk(mli_Func_from_io(&srf->specular_reflection, f));
        chk(mli_Func_from_io(&srf->diffuse_reflection, f));

        return 1;
chk_error:
        return 0;
}
