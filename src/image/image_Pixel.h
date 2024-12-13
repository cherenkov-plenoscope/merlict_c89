/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_IMAGE_PIXEL_H_
#define MLI_IMAGE_PIXEL_H_

#include <stdint.h>
#include "../vector/vector.h"

struct mli_image_Pixel {
        uint16_t col;
        uint16_t row;
};

struct mli_image_Pixel mli_image_Pixel_set_col_row(
        const uint16_t col,
        const uint16_t row);

MLI_VECTOR_DEFINITON(mli_image_PixelVector, struct mli_image_Pixel)

void mli_image_Pixel_fprint(FILE *f, const struct mli_image_Pixel *self);

#endif
