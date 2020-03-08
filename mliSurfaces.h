/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIBOUNDARYLAYER_H_
#define MERLICT_MLIBOUNDARYLAYER_H_

#include <stdint.h>

struct mliSide {
        uint32_t surface;
        uint32_t medium;
};

struct mliBoundaryLayer {
        struct mliSide inner;
        struct mliSide outer;
};

int mliBoundaryLayer_is_equal(
    const struct mliBoundaryLayer a,
    const struct mliBoundaryLayer b)
{
    if (a.inner.surface != b.inner.surface) return 0;
    if (a.outer.surface != b.outer.surface) return 0;
    if (a.inner.medium != b.inner.medium) return 0;
    if (a.outer.medium != b.outer.medium) return 0;
    return 1;
}

#endif
