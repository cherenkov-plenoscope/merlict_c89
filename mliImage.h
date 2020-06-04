/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIIMAGE_H_
#define MERLICT_MLIIMAGE_H_

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "mli_debug.h"
#include "mliColor.h"
#include "mliPixels.h"

struct mliImage {
        uint32_t num_cols;
        uint32_t num_rows;
        struct mliColor *raw;
};

struct mliImage mliImage_init(void)
{
        struct mliImage img;
        img.num_cols = 0u;
        img.num_rows = 0u;
        img.raw = NULL;
        return img;
}

void mliImage_free(struct mliImage *img)
{
        free(img->raw);
        (*img) = mliImage_init();
}

int mliImage_malloc(
        struct mliImage *img,
        const uint32_t num_cols,
        const uint32_t num_rows)
{
        mliImage_free(img);
        img->num_cols = num_cols;
        img->num_rows = num_rows;
        mli_malloc(img->raw, struct mliColor, img->num_cols * img->num_rows);
        return 1;
error:
        mliImage_free(img);
        return 0;
}

uint32_t mliImage_idx(
        const struct mliImage *img,
        const uint32_t col,
        const uint32_t row)
{
        return col * img->num_rows + row;
}

void mliImage_set(
        const struct mliImage *img,
        const uint32_t col,
        const uint32_t row,
        const struct mliColor color)
{
        img->raw[mliImage_idx(img, col, row)] = color;
}

struct mliColor mliImage_at(
        const struct mliImage *img,
        const uint32_t col,
        const uint32_t row)
{
        struct mliColor out;
        out = img->raw[mliImage_idx(img, col, row)];
        return out;
}

int mliImage_malloc_from_ppm(struct mliImage *img, const char *path)
{
        char line[1024];
        uint32_t num_commen_lines = 0;
        uint32_t num_cols;
        uint32_t num_rows;
        uint32_t col;
        uint32_t row;
        FILE *fin;
        fin = fopen(path, "rb");
        mli_check(fin, "Can not open ppm.");
        mli_check(fgets(line, 1024, fin), "Can not read header-line.")
                mli_check(
                        strcmp(line, "P6\n") == 0,
                        "Expected starts with 'P6'.");
        while (1) {
                mli_check(num_commen_lines < 1024, "Expected < 1024 lines.");
                mli_check(fgets(line, 1024, fin), "Can not read header-line.");
                if (line[0] == '#') {
                        num_commen_lines += 1u;
                } else {
                        break;
                }
        }
        num_cols = atoi(line);
        mli_check(fgets(line, 1024, fin), "Can not read header-line.");
        num_rows = atoi(line);
        mli_check(fgets(line, 1024, fin), "Can not read header-line.");
        mli_check(strcmp(line, "255\n") == 0, "Expected 8bit range '255'.");
        mli_check_mem(mliImage_malloc(img, num_cols, num_rows));
        for (row = 0; row < img->num_rows; row++) {
                for (col = 0; col < img->num_cols; col++) {
                        uint8_t r, g, b;
                        struct mliColor color;
                        mli_fread(&r, sizeof(uint8_t), 1u, fin);
                        mli_fread(&g, sizeof(uint8_t), 1u, fin);
                        mli_fread(&b, sizeof(uint8_t), 1u, fin);
                        color.r = (float)r;
                        color.g = (float)g;
                        color.b = (float)b;
                        mliImage_set(img, col, row, color);
                }
        }
        mli_check(!feof(fin), "Unexpected end-of-file.");
        mli_check(!ferror(fin), "File error.");
        fclose(fin);
        return 1;
error:
        mliImage_free(img);
        fclose(fin);
        return 0;
}

int mliImage_write_to_ppm(const struct mliImage *img, const char *path)
{
        FILE *fout;
        uint32_t col;
        uint32_t row;
        fout = fopen(path, "w");
        mli_check(fout, "Can not open ppm-image for writing.");
        fprintf(fout, "P6\n");
        fprintf(fout, "# CREATOR: merlict_c89\n");
        fprintf(fout, "%d\n", img->num_cols);
        fprintf(fout, "%d\n", img->num_rows);
        fprintf(fout, "255\n");
        for (row = 0; row < img->num_rows; row++) {
                for (col = 0; col < img->num_cols; col++) {
                        struct mliColor color = mliImage_at(img, col, row);
                        struct mliColor out = mliColor_truncate_to_uint8(color);
                        uint8_t r = (uint8_t)out.r;
                        uint8_t g = (uint8_t)out.g;
                        uint8_t b = (uint8_t)out.b;
                        fwrite(&r, sizeof(uint8_t), 1u, fout);
                        fwrite(&g, sizeof(uint8_t), 1u, fout);
                        fwrite(&b, sizeof(uint8_t), 1u, fout);
                }
        }
        mli_check(!feof(fout), "Unexpected end-of-file.");
        mli_check(!ferror(fout), "File error.");
        fclose(fout);
        return 1;
error:
        fclose(fout);
        return 0;
}

void mliImage_print_chars(
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
                printf("\n");
        }
}

void mliImage_print(const struct mliImage *img)
{
        const uint64_t num_symbols = 0;
        mliImage_print_chars(img, NULL, NULL, NULL, num_symbols);
}

int mliImage_scale_down_twice(
        const struct mliImage *source,
        struct mliImage *destination)
{
        uint64_t row, col, sr, sc;
        mli_check(
                destination->num_cols * 2u == source->num_cols,
                "Expected destination.num_cols*2u == source.num_cols");
        mli_check(
                destination->num_rows * 2u == source->num_rows,
                "Expected destination.num_rows*2u == source.num_rows");
        for (row = 0; row < destination->num_rows; row++) {
                for (col = 0; col < destination->num_cols; col++) {
                        struct mliColor mix[4];
                        sr = row * 2u;
                        sc = col * 2u;
                        mix[0] = mliImage_at(source, sc + 0, sr + 0);
                        mix[1] = mliImage_at(source, sc + 0, sr + 1);
                        mix[2] = mliImage_at(source, sc + 1, sr + 0);
                        mix[3] = mliImage_at(source, sc + 1, sr + 1);
                        mliImage_set(
                                destination, col, row, mliColor_mean(mix, 4));
                }
        }
        return 1;
error:
        return 0;
}

void mliImage_sobel(const struct mliImage *image, struct mliImage *out)
{
        uint64_t idx_cm1_rp1;
        uint64_t idx_cm1_rp0;
        uint64_t idx_cm1_rm1;

        uint64_t idx_cp1_rp1;
        uint64_t idx_cp1_rp0;
        uint64_t idx_cp1_rm1;

        uint64_t idx_cp0_rp1;
        uint64_t idx_cp0_rm1;

        uint64_t col, row;
        for (col = 1; col < image->num_cols - 1; col++) {
                for (row = 1; row < image->num_rows - 1; row++) {
                        double xr = 0;
                        double xg = 0;
                        double xb = 0;
                        double yr = 0;
                        double yg = 0;
                        double yb = 0;
                        uint64_t idx;

                        idx_cm1_rp1 = mliImage_idx(image, col - 1, row + 1);
                        idx_cm1_rp0 = mliImage_idx(image, col - 1, row);
                        idx_cm1_rm1 = mliImage_idx(image, col - 1, row - 1);

                        idx_cp1_rp1 = mliImage_idx(image, col + 1, row + 1);
                        idx_cp1_rp0 = mliImage_idx(image, col + 1, row);
                        idx_cp1_rm1 = mliImage_idx(image, col + 1, row - 1);

                        idx_cp0_rp1 = mliImage_idx(image, col, row + 1);
                        idx_cp0_rm1 = mliImage_idx(image, col, row - 1);

                        idx = mliImage_idx(out, col, row);

                        xr += -1. * image->raw[idx_cm1_rp1].r;
                        xg += -1. * image->raw[idx_cm1_rp1].g;
                        xb += -1. * image->raw[idx_cm1_rp1].b;

                        xr += -2. * image->raw[idx_cm1_rp0].r;
                        xg += -2. * image->raw[idx_cm1_rp0].g;
                        xb += -2. * image->raw[idx_cm1_rp0].b;

                        xr += -1. * image->raw[idx_cm1_rm1].r;
                        xg += -1. * image->raw[idx_cm1_rm1].g;
                        xb += -1. * image->raw[idx_cm1_rm1].b;

                        xr += +1. * image->raw[idx_cp1_rp1].r;
                        xg += +1. * image->raw[idx_cp1_rp1].g;
                        xb += +1. * image->raw[idx_cp1_rp1].b;

                        xr += +2. * image->raw[idx_cp1_rp0].r;
                        xg += +2. * image->raw[idx_cp1_rp0].g;
                        xb += +2. * image->raw[idx_cp1_rp0].b;

                        xr += +1. * image->raw[idx_cp1_rm1].r;
                        xg += +1. * image->raw[idx_cp1_rm1].g;
                        xb += +1. * image->raw[idx_cp1_rm1].b;

                        yr += -1. * image->raw[idx_cm1_rp1].r;
                        yg += -1. * image->raw[idx_cm1_rp1].g;
                        yb += -1. * image->raw[idx_cm1_rp1].b;

                        yr += -2. * image->raw[idx_cp0_rp1].r;
                        yg += -2. * image->raw[idx_cp0_rp1].g;
                        yb += -2. * image->raw[idx_cp0_rp1].b;

                        yr += -1. * image->raw[idx_cp1_rp1].r;
                        yg += -1. * image->raw[idx_cp1_rp1].g;
                        yb += -1. * image->raw[idx_cp1_rp1].b;

                        yr += +1. * image->raw[idx_cp1_rm1].r;
                        yg += +1. * image->raw[idx_cp1_rm1].g;
                        yb += +1. * image->raw[idx_cp1_rm1].b;

                        yr += +2. * image->raw[idx_cp0_rm1].r;
                        yg += +2. * image->raw[idx_cp0_rm1].g;
                        yb += +2. * image->raw[idx_cp0_rm1].b;

                        yr += +1. * image->raw[idx_cm1_rm1].r;
                        yg += +1. * image->raw[idx_cm1_rm1].g;
                        yb += +1. * image->raw[idx_cm1_rm1].b;

                        out->raw[idx].r = mli_hypot(xr, yr);
                        out->raw[idx].g = mli_hypot(xg, yg);
                        out->raw[idx].b = mli_hypot(xb, yb);
                }
        }
}

void mliImage_luminance_threshold_dilatation(
        const struct mliImage *image,
        const float threshold,
        struct mliImage *out)
{
        const int32_t rows = image->num_rows;
        const int32_t cols = image->num_cols;
        int32_t col, row;
        const struct mliColor color_max = mliColor_set(255., 255., 255.);
        for (row = 0; row < rows; row++) {
                for (col = 0; col < cols; col++) {
                        const struct mliColor color_at =
                                mliImage_at(image, col, row);
                        const float luminance =
                                (color_at.r + color_at.g + color_at.b);
                        if (luminance > threshold) {
                                int32_t orow, ocol;
                                for (orow = -1; orow < 2; orow++) {
                                        for (ocol = -1; ocol < 2; ocol++) {
                                                if (row + orow >= 0 &&
                                                    col + ocol >= 0 &&
                                                    row + orow < rows &&
                                                    col + ocol < cols) {
                                                        mliImage_set(
                                                                out,
                                                                col + ocol,
                                                                row + orow,
                                                                color_max);
                                                }
                                        }
                                }
                        }
                }
        }
}

void mliImage_from_sum_and_exposure(
        const struct mliImage *sum,
        const struct mliImage *exposure,
        struct mliImage *out)
{
        uint64_t pix;
        for (pix = 0u; pix < out->num_rows * out->num_cols; pix++) {
                out->raw[pix].r = sum->raw[pix].r / exposure->raw[pix].r;
                out->raw[pix].g = sum->raw[pix].g / exposure->raw[pix].g;
                out->raw[pix].b = sum->raw[pix].b / exposure->raw[pix].b;
        }
}

int mliPixels_malloc_and_set_from_image(
        struct mliPixels *pixels,
        const struct mliImage *image)
{
        uint64_t i, r, c;
        uint32_t num_pixels = image->num_cols * image->num_rows;
        mli_check_mem(mliPixels_malloc(pixels, num_pixels));
        i = 0u;
        for (r = 0u; r < image->num_rows; r++) {
                for (c = 0u; c < image->num_cols; c++) {
                        pixels->pixels[i].row = r;
                        pixels->pixels[i].col = c;
                        i++;
                }
        }
        return 1;
error:
        return 0;
}

int mliPixels_malloc_from_image_above_threshold(
        struct mliPixels *pixels,
        const struct mliImage *image,
        const float threshold)
{
        uint64_t i, r, c;
        uint32_t num_pixels = 0;
        for (r = 0u; r < image->num_rows; r++) {
                for (c = 0u; c < image->num_cols; c++) {
                        struct mliColor col = mliImage_at(image, c, r);
                        if (col.r + col.g + col.b > threshold) {
                                num_pixels++;
                        }
                }
        }
        mli_check_mem(mliPixels_malloc(pixels, num_pixels));
        i = 0u;
        for (r = 0u; r < image->num_rows; r++) {
                for (c = 0u; c < image->num_cols; c++) {
                        struct mliColor col = mliImage_at(image, c, r);
                        if (col.r + col.g + col.b > threshold) {
                                pixels->pixels[i].row = r;
                                pixels->pixels[i].col = c;
                                i++;
                        }
                }
        }
        return 1;
error:
        return 0;
}

void mliImage_assign_pixel_colors_to_sum_and_exposure_image(
        const struct mliPixels *pixels,
        const struct mliImage *colors,
        struct mliImage *sum_image,
        struct mliImage *exposure_image)
{
        uint64_t pix;
        for (pix = 0u; pix < pixels->num_pixels; pix++) {
                const uint64_t idx = mliImage_idx(
                        sum_image,
                        pixels->pixels[pix].col,
                        pixels->pixels[pix].row);
                sum_image->raw[idx].r += colors->raw[idx].r;
                sum_image->raw[idx].g += colors->raw[idx].g;
                sum_image->raw[idx].b += colors->raw[idx].b;
                exposure_image->raw[idx].r += 1.;
                exposure_image->raw[idx].g += 1.;
                exposure_image->raw[idx].b += 1.;
        }
}

#endif
