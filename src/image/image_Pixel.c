/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "image_Pixel.h"

MLI_VECTOR_IMPLEMENTATION(mli_image_PixelVector, struct mli_image_Pixel)

struct mli_image_Pixel mli_image_Pixel_set_col_row(
        const uint16_t col,
        const uint16_t row)
{
        struct mli_image_Pixel out;
        out.col = col;
        out.row = row;
        return out;
}

void mli_image_Pixel_fprint(FILE *f, const struct mli_image_Pixel *self)
{
        fprintf(f, "(col: %d, row: %d)", self->col, self->row);
}
