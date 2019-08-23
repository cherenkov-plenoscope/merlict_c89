/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIIMAGE_H_
#define MERLICT_MLIIMAGE_H_

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "mli_debug.h"
#include "mliColor.h"

typedef struct {
    uint32_t num_cols;
    uint32_t num_rows;
    mliColor *raw;
} mliImage;

mliImage mliImage_init() {
    mliImage img;
    img.num_cols = 0u;
    img.num_rows = 0u;
    img.raw = NULL;
    return img;}

void mliImage_free(
    mliImage *img) {
    free(img->raw);
    img->raw = NULL;
    img->num_cols = 0u;
    img->num_rows = 0u;}

int mliImage_malloc(
    mliImage *img,
    const uint32_t num_cols,
    const uint32_t num_rows) {
    img->num_cols = num_cols;
    img->num_rows = num_rows;
    mli_malloc(img->raw, mliColor, img->num_cols*img->num_rows);
    return 1;
error:
    mliImage_free(img);
    return 0;}

uint32_t mliImage_idx(
    const mliImage *img,
    const uint32_t col,
    const uint32_t row) {
    return col*img->num_rows + row;}


void mliImage_set(
    const mliImage *img,
    const uint32_t col,
    const uint32_t row,
    const mliColor color) {
    img->raw[mliImage_idx(img, col, row)] = color;}


mliColor mliImage_at(
    const mliImage *img,
    const uint32_t col,
    const uint32_t row) {
    mliColor out;
    out = img->raw[mliImage_idx(img, col, row)];
    return out;}

int mliImage_malloc_from_ppm(
    mliImage *img,
    const char *path) {
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
    mli_check(strcmp(line, "P6\n") == 0, "Expected header to start with 'P6'.");
    while (1) {
        mli_check(num_commen_lines < 1024, "Expected less than 1024 lines.");
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
            mliColor color;
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
    return 0;}


int mliImage_write_to_ppm(const mliImage *img, const char *path) {
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
            mliColor color = mliImage_at(img, col, row);
            mliColor out = mliColor_truncate_to_uint8(color);
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
    return 0;}

void mliImage_print(const mliImage* img) {
    uint32_t col;
    uint32_t row;
    for (row = 0; row < img->num_rows; row = row + 2u) {
        for (col = 0; col < img->num_cols; col++) {
            mliColor color = mliImage_at(img, col, row);
            mliColor out = mliColor_truncate_to_uint8(color);
            uint8_t r = (uint8_t)out.r;
            uint8_t g = (uint8_t)out.g;
            uint8_t b = (uint8_t)out.b;
            printf("\033[48;2;%u;%u;%um \033[0m", r, g, b);
        }
        printf("\n");}}

int mliImage_scale_down_twice(const mliImage *source, mliImage* destination) {
    uint64_t row, col, sr, sc;
    mli_check(
        destination->num_cols*2u == source->num_cols,
        "Expected destination.num_cols*2u == source.num_cols");
    mli_check(destination->num_rows*2u == source->num_rows,
        "Expected destination.num_rows*2u == source.num_rows");
    for (row = 0; row < destination->num_rows; row++) {
        for (col = 0; col < destination->num_cols; col++) {
            mliColor mix[4];
            sr = row*2u;
            sc = col*2u;
            mix[0] = mliImage_at(source, sc + 0, sr + 0);
            mix[1] = mliImage_at(source, sc + 0, sr + 1);
            mix[2] = mliImage_at(source, sc + 1, sr + 0);
            mix[3] = mliImage_at(source, sc + 1, sr + 1);
            mliImage_set(destination, col, row, mliColor_mean(mix, 4));}}
    return 1;
error:
    return 0;}

void mliImage_sobel(const mliImage *image, mliImage* out) {
    uint64_t col, row;
    for (col = 1; col < image->num_cols - 1; col++) {
        for (row = 1; row < image->num_rows - 1; row++) {
            const uint64_t idx_cm1_rp1 = mliImage_idx(image, col - 1, row + 1);
            const uint64_t idx_cm1_rp0 = mliImage_idx(image, col - 1, row    );
            const uint64_t idx_cm1_rm1 = mliImage_idx(image, col - 1, row - 1);

            const uint64_t idx_cp1_rp1 = mliImage_idx(image, col + 1, row + 1);
            const uint64_t idx_cp1_rp0 = mliImage_idx(image, col + 1, row    );
            const uint64_t idx_cp1_rm1 = mliImage_idx(image, col + 1, row - 1);

            const uint64_t idx_cp0_rp1 = mliImage_idx(image, col,     row + 1);
            const uint64_t idx_cp0_rm1 = mliImage_idx(image, col,     row - 1);

            const uint64_t idx = mliImage_idx(out, col, row);

            double xr = 0;
            double xg = 0;
            double xb = 0;

            double yr = 0;
            double yg = 0;
            double yb = 0;

            xr += -1.*image->raw[idx_cm1_rp1].r;
            xg += -1.*image->raw[idx_cm1_rp1].g;
            xb += -1.*image->raw[idx_cm1_rp1].b;

            xr += -2.*image->raw[idx_cm1_rp0].r;
            xg += -2.*image->raw[idx_cm1_rp0].g;
            xb += -2.*image->raw[idx_cm1_rp0].b;

            xr += -1.*image->raw[idx_cm1_rm1].r;
            xg += -1.*image->raw[idx_cm1_rm1].g;
            xb += -1.*image->raw[idx_cm1_rm1].b;

            xr += +1.*image->raw[idx_cp1_rp1].r;
            xg += +1.*image->raw[idx_cp1_rp1].g;
            xb += +1.*image->raw[idx_cp1_rp1].b;

            xr += +2.*image->raw[idx_cp1_rp0].r;
            xg += +2.*image->raw[idx_cp1_rp0].g;
            xb += +2.*image->raw[idx_cp1_rp0].b;

            xr += +1.*image->raw[idx_cp1_rm1].r;
            xg += +1.*image->raw[idx_cp1_rm1].g;
            xb += +1.*image->raw[idx_cp1_rm1].b;

            yr += -1.*image->raw[idx_cm1_rp1].r;
            yg += -1.*image->raw[idx_cm1_rp1].g;
            yb += -1.*image->raw[idx_cm1_rp1].b;

            yr += -2.*image->raw[idx_cp0_rp1].r;
            yg += -2.*image->raw[idx_cp0_rp1].g;
            yb += -2.*image->raw[idx_cp0_rp1].b;

            yr += -1.*image->raw[idx_cp1_rp1].r;
            yg += -1.*image->raw[idx_cp1_rp1].g;
            yb += -1.*image->raw[idx_cp1_rp1].b;

            yr += +1.*image->raw[idx_cp1_rm1].r;
            yg += +1.*image->raw[idx_cp1_rm1].g;
            yb += +1.*image->raw[idx_cp1_rm1].b;

            yr += +2.*image->raw[idx_cp0_rm1].r;
            yg += +2.*image->raw[idx_cp0_rm1].g;
            yb += +2.*image->raw[idx_cp0_rm1].b;

            yr += +1.*image->raw[idx_cm1_rm1].r;
            yg += +1.*image->raw[idx_cm1_rm1].g;
            yb += +1.*image->raw[idx_cm1_rm1].b;

            out->raw[idx].r = mli_hypot(xr, yr);
            out->raw[idx].g = mli_hypot(xg, yg);
            out->raw[idx].b = mli_hypot(xb, yb);
        }
    }
}

#endif
