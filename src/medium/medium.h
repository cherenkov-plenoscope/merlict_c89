/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_MEDIUM_H_
#define MLI_MEDIUM_H_

#include <stdint.h>
#include "../func/func.h"

struct mli_Medium {
        struct mli_Func refraction;
        struct mli_Func absorbtion;
};
struct mli_Medium mli_Medium_init(void);
void mli_Medium_free(struct mli_Medium *medium);

int mli_Medium_equal(const struct mli_Medium *a, const struct mli_Medium *b);

#endif
