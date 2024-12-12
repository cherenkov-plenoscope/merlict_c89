/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "image_Pixel.h"

MLI_VECTOR_IMPLEMENTATION(mli_image_PixelVector, struct mli_image_Pixel)

struct mli_image_Pixel mli_image_Pixel_set(
        const uint16_t row,
        const uint16_t col)
{
        struct mli_image_Pixel out;
        out.row = row;
        out.col = col;
        return out;
}
