/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIMEDIUM_H_
#define MERLICT_C89_MLIMEDIUM_H_

#include <stdint.h>

struct mliMedium {
        uint32_t refraction;
        uint32_t absorbtion;
};

int mliMedium_equal(const struct mliMedium a, const struct mliMedium b);
#endif
