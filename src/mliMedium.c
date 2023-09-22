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

int mliMedium_equal(const struct mliMedium *a, const struct mliMedium *b)
{
        if (!mliFunc_equal(a->refraction, b->refraction))
                return 0;
        if (!mliFunc_equal(a->absorbtion, b->absorbtion))
                return 0;
        return 1;
}
