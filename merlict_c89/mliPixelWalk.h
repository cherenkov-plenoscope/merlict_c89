/* Copyright 2020-2021 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIPIXELWALK_H_
#define MERLICT_C89_MLIPIXELWALK_H_

#include <stdint.h>
#include "mliPixels.h"

struct mliPixelWalk {
        uint32_t chunk_size;
        uint32_t num_chunks_row;
        uint32_t num_chunks_col;
        uint32_t chunk_row;
        uint32_t sub_row;
        uint32_t chunk_col;
        uint32_t sub_col;
        uint32_t num_rows;
        uint32_t num_cols;
        uint32_t i;
};

struct mliPixelWalk mliPixelWalk_set(
        const uint32_t num_cols,
        const uint32_t num_rows,
        const uint32_t chunk_size);
struct mliPixel mliPixelWalk_get(const struct mliPixelWalk *walk);
void mliPixelWalk_walk(struct mliPixelWalk *walk);
#endif
