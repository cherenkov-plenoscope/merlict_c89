/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIMEDIUM_H_
#define MLIMEDIUM_H_

#include <stdint.h>
#include "../func/func.h"

struct mliMedium {
        struct mli_Func refraction;
        struct mli_Func absorbtion;
};
struct mliMedium mliMedium_init(void);
void mliMedium_free(struct mliMedium *medium);

int mliMedium_equal(const struct mliMedium *a, const struct mliMedium *b);

#endif
