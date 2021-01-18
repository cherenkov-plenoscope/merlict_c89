/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIBOUNDARYLAYER_H_
#define MERLICT_C89_MLIBOUNDARYLAYER_H_

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
        const struct mliBoundaryLayer b);

void mliBoundaryLayer_print(const struct mliBoundaryLayer a);
#endif
