/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliMedium_serialize.h"
#include "mliMagicId.h"
#include "mliFunc_serialize.h"
#include "../chk/chk.h"

int mliMedium_fwrite(const struct mliMedium *med, FILE *f)
{
        struct mliMagicId magic = mliMagicId_init();
        /* magic identifier */
        chk(mliMagicId_set(&magic, "mliMedium"));
        chk_fwrite(&magic, sizeof(struct mliMagicId), 1u, f);

        chk(mliFunc_fwrite(&med->refraction, f));
        chk(mliFunc_fwrite(&med->absorbtion, f));

        return 1;
chk_error:
        return 0;
}

int mliMedium_malloc_fread(struct mliMedium *med, FILE *f)
{
        struct mliMagicId magic;
        /* magic identifier */
        chk_fread(&magic, sizeof(struct mliMagicId), 1u, f);
        chk(mliMagicId_has_word(&magic, "mliMedium"));
        mliMagicId_warn_version(&magic);

        chk(mliFunc_malloc_fread(&med->refraction, f));
        chk(mliFunc_malloc_fread(&med->absorbtion, f));

        return 1;
chk_error:
        return 0;
}
