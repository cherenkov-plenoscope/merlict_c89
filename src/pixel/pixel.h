/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_PIXEL_H_
#define MLI_PIXEL_H_

#include <stdint.h>
#include "../vector/vector.h"

struct mli_Pixel {
        uint16_t row;
        uint16_t col;
};

MLI_VECTOR_DEFINITON(mli_Pixels, struct mli_Pixel)

#endif
