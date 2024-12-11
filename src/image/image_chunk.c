/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "image_chunk.h"

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

uint64_t mli_image_Chunk__idx(
        const struct mli_image_Chunk *self,
        const uint64_t col,
        const uint64_t row)
{
        return col * self->edge_size + row;
}
