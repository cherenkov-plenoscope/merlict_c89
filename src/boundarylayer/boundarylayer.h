/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_BOUNDARYLAYER_H_
#define MLI_BOUNDARYLAYER_H_

#include <stdint.h>
#include "../string/string.h"

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

#include "boundarylayer_surface.h"
#include "boundarylayer_medium.h"

struct mli_BoundaryLayer_Side {
        struct mli_BoundaryLayer_Surface surface;
        struct mli_BoundaryLayer_Medium medium;
};

struct mli_BoundaryLayer2 {
        struct mli_BoundaryLayer_Side inner;
        struct mli_BoundaryLayer_Side outer;
        struct mli_String name;
};

void mli_BoundaryLayer2_free(struct mli_BoundaryLayer2 *self);
struct mli_BoundaryLayer2 mli_BoundaryLayer2_init(void);

int mli_BoundaryLayer2_equal(
        const struct mli_BoundaryLayer2 a,
        const struct mli_BoundaryLayer2 b);
#endif
