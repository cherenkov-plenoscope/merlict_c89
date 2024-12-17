/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_BOUNDARYLAYER_H_
#define MLI_BOUNDARYLAYER_H_

#include <stdint.h>

struct mli_boundarylayer_Side {
        uint32_t surface;
        uint32_t medium;
};

struct mli_BoundaryLayer {
        struct mli_boundarylayer_Side inner;
        struct mli_boundarylayer_Side outer;
};

int mli_BoundaryLayer_equal(
        const struct mli_BoundaryLayer a,
        const struct mli_BoundaryLayer b);

void mli_BoundaryLayer_print(const struct mli_BoundaryLayer a);
#endif
