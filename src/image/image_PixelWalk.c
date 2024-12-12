/* Copyright 2020-2021 Sebastian Achim Mueller */
#include "image_PixelWalk.h"

struct mli_image_PixelWalk mli_image_PixelWalk_set(
        const uint32_t num_cols,
        const uint32_t num_rows,
        const uint32_t chunk_edge_size)
{
        struct mli_image_PixelWalk walk;
        uint32_t full_row, full_col, rest_row, rest_col;

        walk.num_cols = num_cols;
        walk.num_rows = num_rows;
        walk.chunk_edge_size = chunk_edge_size;

        walk.i = 0;

        full_row = num_rows / chunk_edge_size;
        full_col = num_cols / chunk_edge_size;
        rest_row = num_rows % chunk_edge_size;
        rest_col = num_cols % chunk_edge_size;

        walk.num_chunks_row = rest_row ? full_row + 1 : full_row;
        walk.num_chunks_col = rest_col ? full_col + 1 : full_col;

        walk.chunk_row = 0u;
        walk.sub_row = 0u;

        walk.chunk_col = 0u;
        walk.sub_col = 0u;

        return walk;
}

struct mli_image_Pixel mli_image_PixelWalk_get_Pixel(
        const struct mli_image_PixelWalk *self)
{
        struct mli_image_Pixel px;
        px.row = self->chunk_row * self->chunk_edge_size + self->sub_row;
        px.col = self->chunk_col * self->chunk_edge_size + self->sub_col;
        return px;
}

void mli_image_PixelWalk_walk(struct mli_image_PixelWalk *self)
{
        struct mli_image_Pixel px;

        self->sub_row += 1u;
        px = mli_image_PixelWalk_get_Pixel(self);
        if (self->sub_row < self->chunk_edge_size && px.row < self->num_rows) {
                return;
        }

        self->sub_row = 0u;
        self->sub_col += 1u;
        px = mli_image_PixelWalk_get_Pixel(self);
        if (self->sub_col < self->chunk_edge_size && px.col < self->num_cols) {
                return;
        }

        self->sub_col = 0u;
        self->chunk_row += 1u;
        if (self->chunk_row < self->num_chunks_row) {
                return;
        }

        self->chunk_row = 0u;
        self->chunk_col += 1u;
        if (self->chunk_col < self->num_chunks_col) {
                return;
        }

        self->chunk_col = 0;
        return;
}
