/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_CORSIKA_HISTOGRAM2D_H_
#define MLI_CORSIKA_HISTOGRAM2D_H_

#include <stdint.h>
#include "../chk/chk.h"
#include "../avl/avl_Dict.h"
#include "../io/io_memory.h"
#include "../vector/vector.h"

struct mli_corsika_Histogram2d {
        struct mli_AvlDict dict;
};

struct mli_corsika_Histogram2dBin {
        int32_t x;
        int32_t y;
        double value;
};

MLI_VECTOR_DEFINITON(
        mliDynCorsikaHistogram2dBin,
        struct mli_corsika_Histogram2dBin)

struct mli_corsika_Histogram2d mli_corsika_Histogram2d_init(void);

chk_rc mli_corsika_Histogram2d_malloc(
        struct mli_corsika_Histogram2d *hist,
        const uint64_t capacity);

void mli_corsika_Histogram2d_free(struct mli_corsika_Histogram2d *hist);

chk_rc mli_corsika_Histogram2d_assign(
        struct mli_corsika_Histogram2d *hist,
        const int32_t x,
        const int32_t y,
        const double weight);

chk_rc mli_corsika_Histogram2d_flatten(
        const struct mli_corsika_Histogram2d *hist,
        struct mliDynCorsikaHistogram2dBin *dump);

void mli_corsika_Histogram2d_reset(struct mli_corsika_Histogram2d *hist);
uint64_t mli_corsika_Histogram2d_len(
        const struct mli_corsika_Histogram2d *hist);

#endif
