/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIPIXELS_H_
#define MLIPIXELS_H_

#include <stdint.h>

struct mliPixel {
        uint16_t row;
        uint16_t col;
};

struct mliPixels {
        uint32_t num_pixels_to_do;
        uint32_t num_pixels;
        struct mliPixel *pixels;
};

int mliPixels_malloc(struct mliPixels *pix, const uint32_t num_pixels);
void mliPixels_free(struct mliPixels *pix);
struct mliPixels mliPixels_init(void);
#endif
