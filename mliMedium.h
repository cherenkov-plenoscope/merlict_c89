/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIMEDIUM_H_
#define MERLICT_MLIMEDIUM_H_

#include <stdint.h>
#include "mli_debug.h"

struct mliMedium {
    uint32_t refraction;
    uint32_t absorbtion;
};

int mliMedium_is_equal(const struct mliMedium a, const struct mliMedium b) {
    if (a.refraction != b.refraction) return 0;
    if (a.absorbtion != b.absorbtion) return 0;
    return 1;
}

#endif
