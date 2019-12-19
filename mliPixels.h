/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIPIXELS_H_
#define MERLICT_MLIPIXELS_H_

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "mli_debug.h"

typedef struct {
    uint16_t row;
    uint16_t col;
} mliPixel;

typedef struct {
    uint32_t num_pixels;
    mliPixel *pixels;
} mliPixels;

mliPixels mliPixels_init() {
    mliPixels pix;
    pix.num_pixels = 0u;
    pix.pixels = NULL;
    return pix;}

void mliPixels_free(mliPixels *pix) {
    free(pix->pixels);
    *pix = mliPixels_init();}

int mliPixels_malloc(mliPixels *pix) {
    mli_malloc(pix->pixels, mliPixel, pix->num_pixels);
    return 1;
error:
    mliPixels_free(pix);
    return 0;}

#endif
