/* Copyright 2020-2021 Sebastian Achim Mueller */
#ifndef MLI_PIXEL_WALK_H_
#define MLI_PIXEL_WALK_H_

#include <stdint.h>
#include "pixel.h"

struct mli_PixelWalk {
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

struct mli_PixelWalk mli_PixelWalk_set(
        const uint32_t num_cols,
        const uint32_t num_rows,
        const uint32_t chunk_size);
struct mli_Pixel mli_PixelWalk_get(const struct mli_PixelWalk *walk);
void mli_PixelWalk_walk(struct mli_PixelWalk *walk);
#endif
