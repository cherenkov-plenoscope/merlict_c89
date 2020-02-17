/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLISURFACES_H_
#define MERLICT_MLISURFACES_H_

#include <stdint.h>

struct mliSurfaces {
    uint32_t inner;
    uint32_t outer;
};

int mliSurfaces_is_equal(
    const struct mliSurfaces a,
    const struct mliSurfaces b)
{
    if (a.inner != b.inner) return 0;
    if (a.outer != b.outer) return 0;
    return 1;
}

#endif
