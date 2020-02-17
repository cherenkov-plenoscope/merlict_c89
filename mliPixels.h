/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIPIXELS_H_
#define MERLICT_MLIPIXELS_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "mli_debug.h"

struct mliPixel {
    uint16_t row;
    uint16_t col;
};

struct mliPixels {
    uint32_t num_pixels;
    struct mliPixel *pixels;
};

struct mliPixels mliPixels_init() {
    struct mliPixels pix;
    pix.num_pixels = 0u;
    pix.pixels = NULL;
    return pix;}

void mliPixels_free(struct mliPixels *pix) {
    free(pix->pixels);
    *pix = mliPixels_init();}

int mliPixels_malloc(struct mliPixels *pix) {
    mli_malloc(pix->pixels, struct mliPixel, pix->num_pixels);
    return 1;
error:
    mliPixels_free(pix);
    return 0;}

#endif
