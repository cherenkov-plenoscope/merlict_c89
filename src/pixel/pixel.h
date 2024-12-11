/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_PIXEL_H_
#define MLI_PIXEL_H_

#include <stdint.h>
#include "../vector/vector.h"

struct mli_image_Pixel {
        uint16_t row;
        uint16_t col;
};

MLI_VECTOR_DEFINITON(mli_image_PixelVector, struct mli_image_Pixel)

#endif
