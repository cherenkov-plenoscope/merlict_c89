/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_IMAGE_CHUNK_H_
#define MLI_IMAGE_CHUNK_H_

#include <stdint.h>
#include "../color/color.h"
#include "image_Pixel.h"

struct mli_image_Chunk {
        uint64_t edge_size;
        struct mli_Color *array;
};
struct mli_image_Chunk mli_image_Chunk_init(void);
void mli_image_Chunk_free(struct mli_image_Chunk *self);
int mli_image_Chunk_malloc(
        struct mli_image_Chunk *self,
        const uint64_t edge_size);
void mli_image_Chunk_set(
        struct mli_image_Chunk *self,
        const uint64_t col,
        const uint64_t row,
        const struct mli_Color color);
struct mli_Color mli_image_Chunk_get(
        const struct mli_image_Chunk *self,
        const uint64_t col,
        const uint64_t row);
struct mli_Color *mli_image_Chunk_get_ptr(
        const struct mli_image_Chunk *self,
        const uint64_t col,
        const uint64_t row);
uint64_t mli_image_Chunk__idx(
        const struct mli_image_Chunk *self,
        const uint64_t col,
        const uint64_t row);

struct mli_image_ChunkGeometry {
        uint64_t num_cols;
        uint64_t num_rows;
        uint64_t chunk_edge_size;
        uint64_t num_chunks_row;
        uint64_t num_chunks_col;
};

struct mli_image_ChunkGeometry mli_image_ChunkGeometry_set(
        const uint64_t num_cols,
        const uint64_t num_rows,
        const uint64_t chunk_edge_size);

int mli_image_ChunkGeometry_equal(
        const struct mli_image_ChunkGeometry a,
        const struct mli_image_ChunkGeometry b);

#endif
