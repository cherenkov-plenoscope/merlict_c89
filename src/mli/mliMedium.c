/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliMedium.h"

struct mliMedium mliMedium_init(void)
{
        struct mliMedium medium;
        medium.refraction = mli_Func_init();
        medium.absorbtion = mli_Func_init();
        return medium;
}

void mliMedium_free(struct mliMedium *medium)
{
        mli_Func_free(&medium->refraction);
        mli_Func_free(&medium->absorbtion);
        (*medium) = mliMedium_init();
}

int mliMedium_equal(const struct mliMedium *a, const struct mliMedium *b)
{
        if (!mli_Func_equal(a->refraction, b->refraction))
                return 0;
        if (!mli_Func_equal(a->absorbtion, b->absorbtion))
                return 0;
        return 1;
}
