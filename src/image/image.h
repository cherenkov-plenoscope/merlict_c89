/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_IMAGE_H_
#define MLI_IMAGE_H_

#include <stdint.h>
#include "../mli/mliColor.h"
#include "../mli/mliPixels.h"

struct mli_Image {
        uint32_t num_cols;
        uint32_t num_rows;
        struct mliColor *raw;
};

void mli_Image_assign_pixel_colors_to_sum_and_exposure_image(
        const struct mliPixels *pixels,
        const struct mli_Image *colors,
        struct mli_Image *sum_image,
        struct mli_Image *exposure_image);
int mliPixels_malloc_from_image_above_threshold(
        struct mliPixels *pixels,
        const struct mli_Image *image,
        const float threshold);
void mliPixels_above_threshold(
        const struct mli_Image *to_do_image,
        const float threshold,
        struct mliPixels *pixels);
void mli_Image_from_sum_and_exposure(
        const struct mli_Image *sum,
        const struct mli_Image *exposure,
        struct mli_Image *out);
void mli_Image_luminance_threshold_dilatation(
        const struct mli_Image *image,
        const float threshold,
        struct mli_Image *out);
void mli_Image_sobel(const struct mli_Image *image, struct mli_Image *out);
int mli_Image_scale_down_twice(
        const struct mli_Image *source,
        struct mli_Image *destination);
struct mliColor mli_Image_at(
        const struct mli_Image *img,
        const uint32_t col,
        const uint32_t row);
void mli_Image_set(
        const struct mli_Image *img,
        const uint32_t col,
        const uint32_t row,
        const struct mliColor color);
void mli_Image_set_all_pixel(
        const struct mli_Image *img,
        const struct mliColor color);
void mliPixels_set_all_from_image(
        struct mliPixels *pixels,
        const struct mli_Image *image);
uint32_t mli_Image_idx(
        const struct mli_Image *img,
        const uint32_t col,
        const uint32_t row);
int mli_Image_malloc(
        struct mli_Image *img,
        const uint32_t num_cols,
        const uint32_t num_rows);
void mli_Image_free(struct mli_Image *img);

void mli_Image_copy(
        const struct mli_Image *source,
        struct mli_Image *destination);
void mli_Image_fabs_difference(
        const struct mli_Image *a,
        const struct mli_Image *b,
        struct mli_Image *out);
struct mli_Image mli_Image_init(void);

void mli_Image_histogram(
        struct mli_Image *img,
        const double *col_bin_edges,
        const double *row_bin_edges,
        const double x,
        const double y,
        const struct mliColor weight);
struct mliColor mli_Image_max(const struct mli_Image *img);
void mli_Image_multiply(struct mli_Image *img, const struct mliColor color);
void mli_Image_divide_pixelwise(
        const struct mli_Image *numerator,
        const struct mli_Image *denominator,
        struct mli_Image *out);
#endif
