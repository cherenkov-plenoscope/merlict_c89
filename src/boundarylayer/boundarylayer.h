/* Copyright 2018-2024 Sebastian Achim Mueller */
#ifndef MLI_BOUNDARYLAYER_H_
#define MLI_BOUNDARYLAYER_H_

#include <stdint.h>
#include "../string/string.h"
#include "../io/io.h"

struct mli_BoundaryLayer_Side {
        uint64_t surface;
        uint64_t medium;
};

struct mli_BoundaryLayer {
        struct mli_BoundaryLayer_Side inner;
        struct mli_BoundaryLayer_Side outer;
        struct mli_String name;
};

void mli_BoundaryLayer_free(struct mli_BoundaryLayer *self);
struct mli_BoundaryLayer mli_BoundaryLayer_init(void);

int mli_BoundaryLayer_equal(
        const struct mli_BoundaryLayer *a,
        const struct mli_BoundaryLayer *b);

int mli_BoundaryLayer_to_io(
        const struct mli_BoundaryLayer *self,
        struct mli_IO *f);
int mli_BoundaryLayer_from_io(struct mli_BoundaryLayer *self, struct mli_IO *f);

#endif
