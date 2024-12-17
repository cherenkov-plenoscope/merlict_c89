/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliMedium_serialize.h"
#include "mliMagicId.h"
#include "../func/func_serialize.h"
#include "../chk/chk.h"

int mliMedium_to_io(const struct mliMedium *med, struct mli_IO *f)
{
        struct mliMagicId magic = mliMagicId_init();
        /* magic identifier */
        chk(mliMagicId_set(&magic, "mliMedium"));
        chk_IO_write(&magic, sizeof(struct mliMagicId), 1u, f);

        chk(mli_Func_to_io(&med->refraction, f));
        chk(mli_Func_to_io(&med->absorbtion, f));

        return 1;
chk_error:
        return 0;
}

int mliMedium_from_io(struct mliMedium *med, struct mli_IO *f)
{
        struct mliMagicId magic;
        /* magic identifier */
        chk_IO_read(&magic, sizeof(struct mliMagicId), 1u, f);
        chk(mliMagicId_has_word(&magic, "mliMedium"));
        mliMagicId_warn_version(&magic);

        chk(mli_Func_from_io(&med->refraction, f));
        chk(mli_Func_from_io(&med->absorbtion, f));

        return 1;
chk_error:
        return 0;
}
