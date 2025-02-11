/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "image_print.h"
#include <stddef.h>
#include <stdio.h>

void mli_Image_print(const struct mli_Image *img, const uint64_t print_mode)
{
        const uint64_t num_symbols = 0;
        mli_Image_print_chars(img, NULL, NULL, NULL, num_symbols, print_mode);
}

void mli_Image_print_chars(
        const struct mli_Image *img,
        const char *symbols,
        const uint64_t *rows,
        const uint64_t *cols,
        const uint64_t num_symbols,
        const uint64_t print_mode)
{
        if (print_mode == MLI_IMAGE_PRINT_ASCII_ESCAPE_COLOR) {
                mli_Image_print_ansi_escape_chars(
                        img, symbols, rows, cols, num_symbols);
        } else {
                mli_Image_print_ascii_chars(
                        img, symbols, rows, cols, num_symbols);
                if (print_mode != MLI_IMAGE_PRINT_ASCII_MONOCHROME) {
                        fprintf(stderr,
                                "Do not know print_mode %u\n",
                                (uint32_t)print_mode);
                }
        }
        return;
}

void mli_Image_print_ansi_escape_chars(
        const struct mli_Image *img,
        const char *symbols,
        const uint64_t *rows,
        const uint64_t *cols,
        const uint64_t num_symbols)
{
        const double color_depth_factor = 255.0;
        uint32_t col, row, sym;
        char symbol;

        for (row = 0; row < mli_Image_num_rows(img); row++) {
                for (col = 0; col < mli_Image_num_cols(img); col++) {
                        uint8_t r, g, b;
                        struct mli_Color color =
                                mli_Image_get_by_col_row(img, col, row);
                        color = mli_Color_multiply(color, color_depth_factor);
                        color = mli_Color_truncate(
                                color, 0., color_depth_factor);
                        r = (uint8_t)color.r;
                        g = (uint8_t)color.g;
                        b = (uint8_t)color.b;
                        symbol = ' ';
                        for (sym = 0; sym < num_symbols; sym++) {
                                if (rows[sym] == row && cols[sym] == col) {
                                        symbol = symbols[sym];
                                        break;
                                }
                        }
                        printf("\033[48;2;%u;%u;%um%c\033[0m", r, g, b, symbol);
                }
                putchar('\n');
        }
        fflush(stdout);
}

void mli_Image_print_ascii_chars(
        const struct mli_Image *img,
        const char *symbols,
        const uint64_t *rows,
        const uint64_t *cols,
        const uint64_t num_symbols)
{
        const double color_depth_factor = 15.0;
        uint32_t col, row, sym;
        char symbol;
        char chars_with_ascending_fill[16] = {
                ' ',
                '.',
                ',',
                '-',
                '~',
                '^',
                '+',
                '=',
                'i',
                'j',
                '$',
                'l',
                'L',
                'Y',
                '8',
                '#'};
        for (row = 0; row < mli_Image_num_rows(img); row++) {
                for (col = 0; col < mli_Image_num_cols(img); col++) {
                        float lum = 0.0;
                        int64_t l = 0;
                        struct mli_Color color =
                                mli_Image_get_by_col_row(img, col, row);
                        color = mli_Color_multiply(color, color_depth_factor);
                        color = mli_Color_truncate(
                                color, 0., color_depth_factor);
                        lum = 1.0 / 3.0 * (color.r + color.g + color.b);
                        l = lum;
                        if (l < 0) {
                                l = 0;
                        }
                        if (l >= 16) {
                                l = 15;
                        }
                        symbol = chars_with_ascending_fill[l];
                        for (sym = 0; sym < num_symbols; sym++) {
                                if (rows[sym] == row && cols[sym] == col) {
                                        symbol = symbols[sym];
                                        break;
                                }
                        }
                        putchar(symbol);
                }
                putchar('\n');
        }
        fflush(stdout);
}
