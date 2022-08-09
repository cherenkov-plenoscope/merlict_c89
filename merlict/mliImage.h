/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIIMAGE_H_
#define MLIIMAGE_H_

#include <stdint.h>
#include "mliColor.h"
#include "mliPixels.h"

struct mliImage {
        uint32_t num_cols;
        uint32_t num_rows;
        struct mliColor *raw;
};

void mliImage_assign_pixel_colors_to_sum_and_exposure_image(
        const struct mliPixels *pixels,
        const struct mliImage *colors,
        struct mliImage *sum_image,
        struct mliImage *exposure_image);
int mliPixels_malloc_from_image_above_threshold(
        struct mliPixels *pixels,
        const struct mliImage *image,
        const float threshold);
void mliPixels_above_threshold(
        const struct mliImage *to_do_image,
        const float threshold,
        struct mliPixels *pixels);
void mliImage_from_sum_and_exposure(
        const struct mliImage *sum,
        const struct mliImage *exposure,
        struct mliImage *out);
void mliImage_luminance_threshold_dilatation(
        const struct mliImage *image,
        const float threshold,
        struct mliImage *out);
void mliImage_sobel(const struct mliImage *image, struct mliImage *out);
int mliImage_scale_down_twice(
        const struct mliImage *source,
        struct mliImage *destination);
struct mliColor mliImage_at(
        const struct mliImage *img,
        const uint32_t col,
        const uint32_t row);
void mliImage_set(
        const struct mliImage *img,
        const uint32_t col,
        const uint32_t row,
        const struct mliColor color);
void mliPixels_set_all_from_image(
        struct mliPixels *pixels,
        const struct mliImage *image);
uint32_t mliImage_idx(
        const struct mliImage *img,
        const uint32_t col,
        const uint32_t row);
int mliImage_malloc(
        struct mliImage *img,
        const uint32_t num_cols,
        const uint32_t num_rows);
void mliImage_free(struct mliImage *img);

void mliImage_copy(const struct mliImage *source, struct mliImage *destination);
void mliImage_fabs_difference(
        const struct mliImage *a,
        const struct mliImage *b,
        struct mliImage *out);
struct mliImage mliImage_init(void);

void mliImage_histogram(
        struct mliImage *img,
        const double *col_bin_edges,
        const double *row_bin_edges,
        const double x,
        const double y,
        const struct mliColor weight);
struct mliColor mliImage_max(const struct mliImage *img);
void mliImage_multiply(struct mliImage *img, const struct mliColor color);
void mliImage_divide_pixelwise(
        const struct mliImage *numerator,
        const struct mliImage *denominator,
        struct mliImage *out);
#endif
