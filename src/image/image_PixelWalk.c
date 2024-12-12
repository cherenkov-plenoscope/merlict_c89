/* Copyright 2020-2021 Sebastian Achim Mueller */
#include "image_PixelWalk.h"
#include "image_chunk.h"

struct mli_image_PixelWalk mli_image_PixelWalk_init(void)
{
        struct mli_image_PixelWalk walk;
        walk.i = 0;

        walk.chunk_row = 0u;
        walk.sub_row = 0u;

        walk.chunk_col = 0u;
        walk.sub_col = 0u;
        return walk;
}

struct mli_image_Pixel mli_image_PixelWalk_get_Pixel(
        const struct mli_image_PixelWalk *self,
        const struct mli_image_ChunkGeometry *chunk_geometry)
{
        struct mli_image_Pixel px;
        const struct mli_image_ChunkGeometry *cg = chunk_geometry;
        px.row = self->chunk_row * cg->chunk_edge_size + self->sub_row;
        px.col = self->chunk_col * cg->chunk_edge_size + self->sub_col;
        return px;
}

void mli_image_PixelWalk_walk(
        struct mli_image_PixelWalk *self,
        const struct mli_image_ChunkGeometry *chunk_geometry)
{
        struct mli_image_Pixel px;
        const struct mli_image_ChunkGeometry *cg = chunk_geometry;

        self->sub_row += 1u;
        px = mli_image_PixelWalk_get_Pixel(self, chunk_geometry);
        if (self->sub_row < cg->chunk_edge_size && px.row < cg->num_rows) {
                return;
        }

        self->sub_row = 0u;
        self->sub_col += 1u;
        px = mli_image_PixelWalk_get_Pixel(self, chunk_geometry);
        if (self->sub_col < cg->chunk_edge_size && px.col < cg->num_cols) {
                return;
        }

        self->sub_col = 0u;
        self->chunk_row += 1u;
        if (self->chunk_row < cg->num_chunks_row) {
                return;
        }

        self->chunk_row = 0u;
        self->chunk_col += 1u;
        if (self->chunk_col < cg->num_chunks_col) {
                return;
        }

        self->chunk_col = 0;
        return;
}

struct mli_image_PixelWalk mli_image_PixelWalk_from_pixel(
        const struct mli_image_ChunkGeometry *geometry,
        struct mli_image_Pixel pixel)
{
        struct mli_image_PixelWalk address;
        address.chunk_row = pixel.row / geometry->num_chunks_row;
        address.sub_row = pixel.row % geometry->num_chunks_row;
        address.chunk_col = pixel.col / geometry->num_chunks_col;
        address.sub_col = pixel.col % geometry->num_chunks_col;
        return address;
}
