/* Copyright 2020-2021 Sebastian Achim Mueller */
#ifndef MLIPIXELWALK_H_
#define MLIPIXELWALK_H_

#include <stdint.h>
#include "mliPixels.h"

struct mliPixelWalk {
        /*
         * PixelWalk walks over the pixels of a rectangular image in a
         * cache-aware-way with respect to raytracing.
         * The goal is to bundle rays that will go to similar directions.
         * Instead of running fast along one axis of the image, and slow along
         * the other, PixelWalk spreads the walk among both axis by walking
         * small quadratic chunks of pixels.
         */
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
