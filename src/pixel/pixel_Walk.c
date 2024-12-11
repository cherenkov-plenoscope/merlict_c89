/* Copyright 2020-2021 Sebastian Achim Mueller */
#include "pixel_Walk.h"

struct mli_image_PixelWalk mli_image_PixelWalk_set(
        const uint32_t num_cols,
        const uint32_t num_rows,
        const uint32_t chunk_size)
{
        struct mli_image_PixelWalk walk;
        uint32_t full_row, full_col, rest_row, rest_col;

        walk.num_cols = num_cols;
        walk.num_rows = num_rows;
        walk.chunk_size = chunk_size;

        walk.i = 0;

        full_row = num_rows / chunk_size;
        full_col = num_cols / chunk_size;
        rest_row = num_rows % chunk_size;
        rest_col = num_cols % chunk_size;

        walk.num_chunks_row = rest_row ? full_row + 1 : full_row;
        walk.num_chunks_col = rest_col ? full_col + 1 : full_col;

        walk.chunk_row = 0u;
        walk.sub_row = 0u;

        walk.chunk_col = 0u;
        walk.sub_col = 0u;

        return walk;
}

struct mli_image_Pixel mli_image_PixelWalk_get(
        const struct mli_image_PixelWalk *walk)
{
        struct mli_image_Pixel px;
        px.row = walk->chunk_row * walk->chunk_size + walk->sub_row;
        px.col = walk->chunk_col * walk->chunk_size + walk->sub_col;
        return px;
}

void mli_image_PixelWalk_walk(struct mli_image_PixelWalk *walk)
{
        struct mli_image_Pixel px;

        walk->sub_row += 1u;
        px = mli_image_PixelWalk_get(walk);
        if (walk->sub_row < walk->chunk_size && px.row < walk->num_rows) {
                return;
        }

        walk->sub_row = 0u;
        walk->sub_col += 1u;
        px = mli_image_PixelWalk_get(walk);
        if (walk->sub_col < walk->chunk_size && px.col < walk->num_cols) {
                return;
        }

        walk->sub_col = 0u;
        walk->chunk_row += 1u;
        if (walk->chunk_row < walk->num_chunks_row) {
                return;
        }

        walk->chunk_row = 0u;
        walk->chunk_col += 1u;
        if (walk->chunk_col < walk->num_chunks_col) {
                return;
        }

        walk->chunk_col = 0;
        return;
}
