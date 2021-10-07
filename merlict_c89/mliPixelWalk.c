/* Copyright 2020-2021 Sebastian Achim Mueller */
#include "mliPixelWalk.h"

struct mliPixelWalk mliPixelWalk_set(
        const uint32_t num_cols,
        const uint32_t num_rows,
        const uint32_t chunk_size)
{
        struct mliPixelWalk walk;
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

struct mliPixel mliPixelWalk_get(const struct mliPixelWalk *walk)
{
        struct mliPixel px;
        px.row = walk->chunk_row * walk->chunk_size + walk->sub_row;
        px.col = walk->chunk_col * walk->chunk_size + walk->sub_col;
        return px;
}

void mliPixelWalk_walk(struct mliPixelWalk *walk)
{
        struct mliPixel px;

        walk->sub_row += 1u;
        px = mliPixelWalk_get(walk);
        if (walk->sub_row < walk->chunk_size && px.row < walk->num_rows) {
                return;
        }

        walk->sub_row = 0u;
        walk->sub_col += 1u;
        px = mliPixelWalk_get(walk);
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
