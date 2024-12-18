/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "image_chunk.h"
#include <assert.h>
#include <stdlib.h>
#include "../chk/chk.h"

struct mli_image_Chunk mli_image_Chunk_init(void)
{
        struct mli_image_Chunk out;
        out.edge_size = 0;
        out.array = NULL;
        return out;
}

void mli_image_Chunk_free(struct mli_image_Chunk *self)
{
        free(self->array);
        (*self) = mli_image_Chunk_init();
}

int mli_image_Chunk_malloc(
        struct mli_image_Chunk *self,
        const uint64_t edge_size)
{
        const uint64_t num_pixel = edge_size * edge_size;
        self->edge_size = edge_size;
        chk_malloc(self->array, struct mli_Color, num_pixel);
        return 1;
chk_error:
        return 0;
}

void mli_image_Chunk_set(
        struct mli_image_Chunk *self,
        const uint64_t col,
        const uint64_t row,
        const struct mli_Color color)
{
        self->array[mli_image_Chunk__idx(self, col, row)] = color;
}

struct mli_Color mli_image_Chunk_get(
        const struct mli_image_Chunk *self,
        const uint64_t col,
        const uint64_t row)
{
        return self->array[mli_image_Chunk__idx(self, col, row)];
}

struct mli_Color *mli_image_Chunk_get_ptr(
        const struct mli_image_Chunk *self,
        const uint64_t col,
        const uint64_t row)
{
        return &self->array[mli_image_Chunk__idx(self, col, row)];
}

uint64_t mli_image_Chunk__idx(
        const struct mli_image_Chunk *self,
        const uint64_t col,
        const uint64_t row)
{
        return col * self->edge_size + row;
}

struct mli_image_ChunkGeometry mli_image_ChunkGeometry_set(
        const uint64_t num_cols,
        const uint64_t num_rows,
        const uint64_t chunk_edge_size)
{
        struct mli_image_ChunkGeometry out;
        uint64_t full_row, full_col, rest_row, rest_col;

        assert(chunk_edge_size > 0);

        out.num_cols = num_cols;
        out.num_rows = num_rows;
        out.chunk_edge_size = chunk_edge_size;

        full_row = num_rows / chunk_edge_size;
        full_col = num_cols / chunk_edge_size;
        rest_row = num_rows % chunk_edge_size;
        rest_col = num_cols % chunk_edge_size;

        out.num_chunks_row = rest_row ? full_row + 1 : full_row;
        out.num_chunks_col = rest_col ? full_col + 1 : full_col;
        return out;
}

int mli_image_ChunkGeometry_equal(
        const struct mli_image_ChunkGeometry a,
        const struct mli_image_ChunkGeometry b)
{
        if (a.num_cols != b.num_cols)
                return 0;
        if (a.num_rows != b.num_rows)
                return 0;
        if (a.chunk_edge_size != b.chunk_edge_size)
                return 0;
        if (a.num_chunks_row != b.num_chunks_row)
                return 0;
        if (a.num_chunks_col != b.num_chunks_col)
                return 0;
        return 1;
}
