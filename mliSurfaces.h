/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLISURFACES_H_
#define MERLICT_MLISURFACES_H_

#include <stdint.h>

struct mliSide {
        uint32_t surface;
        uint32_t medium;
};

struct mliSurfaces {
        struct mliSide inner;
        struct mliSide outer;
};

int mliSurfaces_is_equal(
    const struct mliSurfaces a,
    const struct mliSurfaces b)
{
    if (a.inner.surface != b.inner.surface) return 0;
    if (a.outer.surface != b.outer.surface) return 0;
    if (a.inner.medium != b.inner.medium) return 0;
    if (a.outer.medium != b.outer.medium) return 0;
    return 1;
}

#endif
