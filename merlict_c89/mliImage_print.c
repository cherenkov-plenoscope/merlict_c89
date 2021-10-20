/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliImage_print.h"

void mliImage_print(const struct mliImage *img, const uint64_t print_mode)
{
        const uint64_t num_symbols = 0;
        mliImage_print_chars(img, NULL, NULL, NULL, num_symbols, print_mode);
}

void mliImage_print_chars(
        const struct mliImage *img,
        const char *symbols,
        const uint64_t *rows,
        const uint64_t *cols,
        const uint64_t num_symbols,
        const uint64_t print_mode)
{
        if (print_mode == MLI_ANSI_ESCAPE_COLOR) {
                mliImage_print_ansi_escape_chars(
                        img, symbols, rows, cols, num_symbols);
        } else {
                mliImage_print_ascii_chars(
                        img, symbols, rows, cols, num_symbols);
                if (print_mode != MLI_ASCII_MONOCHROME) {
                        fprintf(stderr,
                                "Do not know print_mode %u\n",
                                (uint32_t)print_mode);
                }
        }
        return;
}

void mliImage_print_ansi_escape_chars(
        const struct mliImage *img,
        const char *symbols,
        const uint64_t *rows,
        const uint64_t *cols,
        const uint64_t num_symbols)
{
        uint32_t col, row, sym;
        char symbol;
        for (row = 0; row < img->num_rows; row++) {
                for (col = 0; col < img->num_cols; col++) {
                        struct mliColor color = mliImage_at(img, col, row);
                        struct mliColor out = mliColor_truncate_to_uint8(color);
                        uint8_t r = (uint8_t)out.r;
                        uint8_t g = (uint8_t)out.g;
                        uint8_t b = (uint8_t)out.b;
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

void mliImage_print_ascii_chars(
        const struct mliImage *img,
        const char *symbols,
        const uint64_t *rows,
        const uint64_t *cols,
        const uint64_t num_symbols)
{
        uint32_t col, row, sym;
        char symbol;
        char chars_with_ascending_fill[16] = {' ',
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
        for (row = 0; row < img->num_rows; row++) {
                for (col = 0; col < img->num_cols; col++) {
                        struct mliColor color = mliImage_at(img, col, row);
                        struct mliColor out = mliColor_truncate_to_uint8(color);
                        float lum = 1.0 / 3.0 * (out.r + out.g + out.b);
                        int64_t l = lum / 16.0;
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
