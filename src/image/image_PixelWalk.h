/* Copyright 2020-2021 Sebastian Achim Mueller */
#ifndef MLI_IMAGE_PIXELWALK_H_
#define MLI_IMAGE_PIXELWALK_H_

#include <stdint.h>
#include "image_Pixel.h"
#include "image_chunk.h"

struct mli_image_PixelWalk {
        /*
         * PixelWalk walks over the pixels of a rectangular image in a
         * cache-aware-way with respect to raytracing.
         * The goal is to bundle rays that will go to similar directions.
         * Instead of running fast along one axis of the image, and slow along
         * the other, PixelWalk spreads the walk among both axis by walking
         * small quadratic chunks of pixels.
         */
        uint32_t chunk_row;
        uint32_t sub_row;
        uint32_t chunk_col;
        uint32_t sub_col;
        uint32_t i;
};

struct mli_image_PixelWalk mli_image_PixelWalk_init(void);
struct mli_image_Pixel mli_image_PixelWalk_get_Pixel(
        const struct mli_image_PixelWalk *self,
        const struct mli_image_ChunkGeometry *chunk_geometry);
struct mli_image_Pixel mli_image_PixelWalk_get_Pixel(
        const struct mli_image_PixelWalk *self,
        const struct mli_image_ChunkGeometry *chunk_geometry);
void mli_image_PixelWalk_walk(
        struct mli_image_PixelWalk *self,
        const struct mli_image_ChunkGeometry *chunk_geometry);

struct mli_image_PixelWalk mli_image_PixelWalk_from_pixel(
        const struct mli_image_ChunkGeometry *geometry,
        struct mli_image_Pixel pixel);

void mli_image_PixelWalk_fprint(
        FILE *f,
        const struct mli_image_PixelWalk *self);

#endif
