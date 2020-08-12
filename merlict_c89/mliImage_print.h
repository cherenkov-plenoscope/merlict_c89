/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIIMAGE_PRINT_H_
#define MERLICT_C89_MLIIMAGE_PRINT_H_

#include <stdint.h>
#include "mliImage.h"

#define MLI_ASCII_MONOCHROME 100
#define MLI_ANSI_ESCAPE_COLOR 101

void mliImage_print(const struct mliImage *img, const uint64_t print_mode);
void mliImage_print_chars(
        const struct mliImage *img,
        const char *symbols,
        const uint64_t *rows,
        const uint64_t *cols,
        const uint64_t num_symbols,
        const uint64_t print_mode);

/* Colored ANSI escape sequences */

void _mliImage_print_ansi_escape_chars(
        const struct mliImage *img,
        const char *symbols,
        const uint64_t *rows,
        const uint64_t *cols,
        const uint64_t num_symbols);

/* Monochrome ASCII chars */

void _mliImage_print_ascii_chars(
        const struct mliImage *img,
        const char *symbols,
        const uint64_t *rows,
        const uint64_t *cols,
        const uint64_t num_symbols);
#endif
