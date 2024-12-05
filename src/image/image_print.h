/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_IMAGE_PRINT_H_
#define MLI_IMAGE_PRINT_H_

#include <stdint.h>
#include "../image/image.h"

#define MLI_ASCII_MONOCHROME 100
#define MLI_ANSI_ESCAPE_COLOR 101

void mli_Image_print(const struct mli_Image *img, const uint64_t print_mode);
void mli_Image_print_chars(
        const struct mli_Image *img,
        const char *symbols,
        const uint64_t *rows,
        const uint64_t *cols,
        const uint64_t num_symbols,
        const uint64_t print_mode);

/* Colored ANSI escape sequences */

void mli_Image_print_ansi_escape_chars(
        const struct mli_Image *img,
        const char *symbols,
        const uint64_t *rows,
        const uint64_t *cols,
        const uint64_t num_symbols);

/* Monochrome ASCII chars */

void mli_Image_print_ascii_chars(
        const struct mli_Image *img,
        const char *symbols,
        const uint64_t *rows,
        const uint64_t *cols,
        const uint64_t num_symbols);
#endif
