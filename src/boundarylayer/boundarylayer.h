/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_BOUNDARYLAYER_H_
#define MLI_BOUNDARYLAYER_H_

#include <stdint.h>
#include "../string/string.h"
#include "../io/io.h"

#include "boundarylayer_surface.h"
#include "boundarylayer_medium.h"

struct mli_BoundaryLayer_Side {
        uint64_t surface;
        uint64_t medium;
};

struct mli_BoundaryLayer2 {
        struct mli_BoundaryLayer_Side inner;
        struct mli_BoundaryLayer_Side outer;
        struct mli_String name;
};

void mli_BoundaryLayer2_free(struct mli_BoundaryLayer2 *self);
struct mli_BoundaryLayer2 mli_BoundaryLayer2_init(void);

int mli_BoundaryLayer2_equal(
        const struct mli_BoundaryLayer2 *a,
        const struct mli_BoundaryLayer2 *b);

int mli_BoundaryLayer2_to_io(
        const struct mli_BoundaryLayer2 *self,
        struct mli_IO *f);
int mli_BoundaryLayer2_from_io(
        struct mli_BoundaryLayer2 *self,
        struct mli_IO *f);

#endif
