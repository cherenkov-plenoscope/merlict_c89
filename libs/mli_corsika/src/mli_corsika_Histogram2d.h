/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_CORSIKA_HISTOGRAM2D_H_
#define MLI_CORSIKA_HISTOGRAM2D_H_

#include <stdint.h>
#include "../../mli/src/mliAvlDict.h"
#include "../../mli/src/mliIo.h"
#include "../../mtl/src/vector.h"

struct mliCorsikaHistogram2d {
        struct mliAvlDict dict;
};

struct mliCorsikaHistogram2dBin {
        int32_t x;
        int32_t y;
        double value;
};

MTL_VEC_DEFINITON(mli, CorsikaHistogram2dBin, struct mliCorsikaHistogram2dBin)

struct mliCorsikaHistogram2d mliCorsikaHistogram2d_init(void);

int mliCorsikaHistogram2d_malloc(
        struct mliCorsikaHistogram2d *hist,
        const uint64_t capacity);

void mliCorsikaHistogram2d_free(struct mliCorsikaHistogram2d *hist);

int mliCorsikaHistogram2d_assign(
        struct mliCorsikaHistogram2d *hist,
        const int32_t x,
        const int32_t y,
        const double weight);

int mliCorsikaHistogram2d_flatten(
        const struct mliCorsikaHistogram2d *hist,
        struct mliDynCorsikaHistogram2dBin *dump);

void mliCorsikaHistogram2d_reset(struct mliCorsikaHistogram2d *hist);
uint64_t mliCorsikaHistogram2d_len(const struct mliCorsikaHistogram2d *hist);

#endif
