/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIMEDIUM_H_
#define MERLICT_C89_MLIMEDIUM_H_

#include <stdint.h>

struct mliMedium {
        struct mliFunc refraction;
        struct mliFunc absorbtion;
};
struct mliMedium mliMedium_init(void);
void mliMedium_free(struct mliMedium *medium);

int mliMedium_equal(const struct mliMedium *a, const struct mliMedium *b);

#endif
