/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_IMAGE_H_
#define MLI_IMAGE_H_

#include <stdint.h>
#include "../chk/chk.h"
#include "../color/color.h"
#include "image_Pixel.h"
#include "image_PixelVector.h"
#include "image_chunk.h"
#include "image_PixelWalk.h"

struct mli_Image {
        struct mli_image_ChunkGeometry geometry;
        struct mli_image_Chunk **chunks;
};

struct mli_Image mli_Image_init(void);

void mli_Image_free(struct mli_Image *self);

chk_rc mli_Image_malloc(
        struct mli_Image *self,
        const uint32_t num_cols,
        const uint32_t num_rows);

chk_rc mli_Image_malloc_same_size(
        struct mli_Image *self,
        const struct mli_Image *other);

chk_rc mli_Image_copy(const struct mli_Image *src, struct mli_Image *dst);

chk_rc mli_Image__malloc(
        struct mli_Image *self,
        const uint32_t num_cols,
        const uint32_t num_rows);

uint64_t mli_Image_num_pixel(const struct mli_Image *self);
uint64_t mli_Image_num_cols(const struct mli_Image *self);
uint64_t mli_Image_num_rows(const struct mli_Image *self);

void mli_Image__set_by_PixelWalk(
        const struct mli_Image *self,
        const struct mli_image_PixelWalk walk,
        const struct mli_Color color);
struct mli_Color mli_Image__get_by_PixelWalk(
        const struct mli_Image *self,
        const struct mli_image_PixelWalk walk);
struct mli_Color *mli_Image__get_ptr_by_PixelWalk(
        const struct mli_Image *self,
        const struct mli_image_PixelWalk walk);

void mli_Image_set_by_Pixel(
        struct mli_Image *self,
        const struct mli_image_Pixel px,
        const struct mli_Color color);
struct mli_Color mli_Image_get_by_Pixel(
        const struct mli_Image *self,
        const struct mli_image_Pixel px);

void mli_Image_set_by_col_row(
        struct mli_Image *self,
        const uint32_t col,
        const uint32_t row,
        const struct mli_Color color);
struct mli_Color mli_Image_get_by_col_row(
        const struct mli_Image *self,
        const uint32_t col,
        const uint32_t row);

chk_rc mli_image_PixelVector_above_threshold(
        const struct mli_Image *to_do_image,
        const float threshold,
        struct mli_image_PixelVector *pixels);

chk_rc mli_Image_luminance_threshold_dilatation(
        const struct mli_Image *self,
        const float threshold,
        const struct mli_Color marker,
        struct mli_Image *out);

chk_rc mli_Image_sobel(const struct mli_Image *src, struct mli_Image *dst);

chk_rc mli_Image_scale_down_twice(
        const struct mli_Image *source,
        struct mli_Image *destination);

void mli_Image_set_all(
        const struct mli_Image *img,
        const struct mli_Color color);
void mli_image_PixelVector_push_back_all_from_image(
        struct mli_image_PixelVector *pixels,
        const struct mli_Image *image);

chk_rc mli_Image_fabs_difference(
        const struct mli_Image *a,
        const struct mli_Image *b,
        struct mli_Image *out);

void mli_Image_histogram(
        struct mli_Image *img,
        const double *col_bin_edges,
        const double *row_bin_edges,
        const double x,
        const double y,
        const struct mli_Color weight);

struct mli_Color mli_Image_max(const struct mli_Image *img);

void mli_Image_multiply(struct mli_Image *img, const struct mli_Color color);
void mli_Image_power(struct mli_Image *self, const struct mli_Color power);

chk_rc mli_Image_divide_pixelwise(
        const struct mli_Image *numerator,
        const struct mli_Image *denominator,
        struct mli_Image *out);
#endif
