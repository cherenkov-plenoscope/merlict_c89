/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIIMAGE_H_
#define MERLICT_MLIIMAGE_H_

#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "mliColor.h"

typedef struct {
    uint32_t num_cols;
    uint32_t num_rows;
    mliColor *raw;
} mliImage;


void mliImage_init(
    mliImage *img,
    const uint32_t num_cols,
    const uint32_t num_rows) {
    img->num_cols = num_cols;
    img->num_rows = num_rows;
    img->raw = (mliColor*)malloc(
        img->num_cols*img->num_rows * sizeof(mliColor));}


void mliImage_free(
    mliImage *img) {
    free(img->raw);
    img->num_cols = 0u;
    img->num_rows = 0u;}


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


int mliImage_init_from_ppm(
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
    if (fin == NULL) goto close_and_exit_failure;
    if (fgets(line, 1024, fin) == NULL) goto close_and_exit_failure;
    if (strcmp(line, "P6\n") != 0) goto close_and_exit_failure;
    while (1) {
        if (num_commen_lines > 1024) goto close_and_exit_failure;
        if (fgets(line, 1024, fin) == NULL) goto close_and_exit_failure;
        if (line[0] == '#') {
            num_commen_lines += 1u;
        } else {
            break;
        }
    }
    num_cols = atoi(line);
    if (fgets(line, 1024, fin) == NULL) goto close_and_exit_failure;
    num_rows = atoi(line);
    if (fgets(line, 1024, fin) == NULL) goto close_and_exit_failure;
    if (strcmp(line, "255\n") != 0) goto close_and_exit_failure;
    mliImage_init(img, num_cols, num_rows);
    for (row = 0; row < img->num_rows; row++) {
        for (col = 0; col < img->num_cols; col++) {
            uint8_t r, g, b;
            mliColor color;
            fread(&r, sizeof(uint8_t), 1u, fin);
            fread(&g, sizeof(uint8_t), 1u, fin);
            fread(&b, sizeof(uint8_t), 1u, fin);
            color.r = (float)r;
            color.g = (float)g;
            color.b = (float)b;
            mliImage_set(img, col, row, color);
        }
    }
    fclose(fin);
    return EXIT_SUCCESS;

    close_and_exit_failure:
    fclose(fin);
    return EXIT_FAILURE;}


int mliImage_write_to_ppm(const mliImage *img, const char *path) {
    FILE *fout;
    uint32_t col;
    uint32_t row;
    fout = fopen(path, "w");
    if (fout == NULL) goto close_and_exit_failure;
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
    fclose(fout);
    return EXIT_SUCCESS;

    close_and_exit_failure:
    fclose(fout);
    return EXIT_FAILURE;}

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


#endif
