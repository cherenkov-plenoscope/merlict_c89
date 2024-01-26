/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliPixels.h"
#include "chk.h"

struct mliPixels mliPixels_init(void)
{
        struct mliPixels pix;
        pix.num_pixels_to_do = 0u;
        pix.num_pixels = 0u;
        pix.pixels = NULL;
        return pix;
}

void mliPixels_free(struct mliPixels *pix)
{
        free(pix->pixels);
        *pix = mliPixels_init();
}

int mliPixels_malloc(struct mliPixels *pix, const uint32_t num_pixels)
{
        mliPixels_free(pix);
        pix->num_pixels = num_pixels;
        chk_malloc(pix->pixels, struct mliPixel, pix->num_pixels);
        return 1;
error:
        mliPixels_free(pix);
        return 0;
}
