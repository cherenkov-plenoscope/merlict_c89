/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "medium.h"

struct mli_Medium mli_Medium_init(void)
{
        struct mli_Medium medium;
        medium.refraction = mli_Func_init();
        medium.absorbtion = mli_Func_init();
        return medium;
}

void mli_Medium_free(struct mli_Medium *medium)
{
        mli_Func_free(&medium->refraction);
        mli_Func_free(&medium->absorbtion);
        (*medium) = mli_Medium_init();
}

int mli_Medium_equal(const struct mli_Medium *a, const struct mli_Medium *b)
{
        if (!mli_Func_equal(a->refraction, b->refraction))
                return 0;
        if (!mli_Func_equal(a->absorbtion, b->absorbtion))
                return 0;
        return 1;
}
