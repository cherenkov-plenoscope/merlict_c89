/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliMedium.h"

struct mliMedium mliMedium_init(void)
{
        struct mliMedium medium;
        medium.refraction = mliFunc_init();
        medium.absorbtion = mliFunc_init();
        return medium;
}

void mliMedium_free(struct mliMedium *medium)
{
        mliFunc_free(&medium->refraction);
        mliFunc_free(&medium->absorbtion);
        (*medium) = mliMedium_init();
}

int mliMedium_malloc(
        struct mliMedium *medium,
        const uint32_t num_points_refraction,
        const uint32_t num_points_absorbtion)
{
        mliMedium_free(medium);
        chk(mliFunc_malloc(&medium->refraction, num_points_refraction));
        chk(mliFunc_malloc(&medium->absorbtion, num_points_absorbtion));
        return 1;
error:
        return 0;
}

int mliMedium_equal(const struct mliMedium *a, const struct mliMedium *b)
{
        if (!mliFunc_equal(a->refraction, b->refraction))
                return 0;
        if (!mliFunc_equal(a->absorbtion, b->absorbtion))
                return 0;
        return 1;
}

int mliMedium_fwrite(const struct mliMedium *med, FILE *f)
{
        struct mliMagicId magic = mliMagicId_init();
        /* magic identifier */
        chk(mliMagicId_set(&magic, "mliMedium"));
        chk_fwrite(&magic, sizeof(struct mliMagicId), 1u, f);

        chk(mliFunc_fwrite(&med->refraction, f));
        chk(mliFunc_fwrite(&med->absorbtion, f));

        return 1;
error:
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
error:
        return 0;
}
