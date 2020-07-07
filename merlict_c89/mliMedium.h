/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIMEDIUM_H_
#define MERLICT_C89_MLIMEDIUM_H_

#include <stdint.h>

#include "mli_debug.h"

struct mliMedium {
        uint32_t refraction;
        uint32_t absorbtion;
};

int mliMedium_is_equal(const struct mliMedium a, const struct mliMedium b);
#endif
