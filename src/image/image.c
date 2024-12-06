/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "../image/image.h"
#include <assert.h>
#include "../pixel/pixel_Walk.h"
#include "../math/math.h"
#include "../chk/chk.h"
#include <float.h>

struct mli_Image mli_Image_init(void)
{
        struct mli_Image img;
        img.num_cols = 0u;
        img.num_rows = 0u;
        img.raw = NULL;
        return img;
}

void mli_Image_free(struct mli_Image *img)
{
        free(img->raw);
        (*img) = mli_Image_init();
}

int mli_Image_malloc(
        struct mli_Image *img,
        const uint32_t num_cols,
        const uint32_t num_rows)
{
        mli_Image_free(img);
        img->num_cols = num_cols;
        img->num_rows = num_rows;
        chk_malloc(img->raw, struct mli_Color, img->num_cols * img->num_rows);
        return 1;
chk_error:
        mli_Image_free(img);
        return 0;
}

uint32_t mli_Image_idx(
        const struct mli_Image *img,
        const uint32_t col,
        const uint32_t row)
{
        return col * img->num_rows + row;
}

void mli_Image_set(
        const struct mli_Image *img,
        const uint32_t col,
        const uint32_t row,
        const struct mli_Color color)
{
        img->raw[mli_Image_idx(img, col, row)] = color;
}

void mli_Image_set_all_pixel(
        const struct mli_Image *img,
        const struct mli_Color color)
{
        uint64_t row, col;
        for (row = 0; row < img->num_rows; row++) {
                for (col = 0; col < img->num_cols; col++) {
                        img->raw[mli_Image_idx(img, col, row)] = color;
                }
        }
}

struct mli_Color mli_Image_at(
        const struct mli_Image *img,
        const uint32_t col,
        const uint32_t row)
{
        struct mli_Color out;
        out = img->raw[mli_Image_idx(img, col, row)];
        return out;
}

int mli_Image_scale_down_twice(
        const struct mli_Image *source,
        struct mli_Image *destination)
{
        uint64_t row, col, sr, sc;
        chk_msg(destination->num_cols * 2u == source->num_cols,
                "Expected destination.num_cols*2u == source.num_cols");
        chk_msg(destination->num_rows * 2u == source->num_rows,
                "Expected destination.num_rows*2u == source.num_rows");
        for (row = 0; row < destination->num_rows; row++) {
                for (col = 0; col < destination->num_cols; col++) {
                        struct mli_Color mix[4];
                        sr = row * 2u;
                        sc = col * 2u;
                        mix[0] = mli_Image_at(source, sc + 0, sr + 0);
                        mix[1] = mli_Image_at(source, sc + 0, sr + 1);
                        mix[2] = mli_Image_at(source, sc + 1, sr + 0);
                        mix[3] = mli_Image_at(source, sc + 1, sr + 1);
                        mli_Image_set(
                                destination, col, row, mli_Color_mean(mix, 4));
                }
        }
        return 1;
chk_error:
        return 0;
}

void mli_Image_sobel(const struct mli_Image *image, struct mli_Image *out)
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

                        idx_cm1_rp1 = mli_Image_idx(image, col - 1, row + 1);
                        idx_cm1_rp0 = mli_Image_idx(image, col - 1, row);
                        idx_cm1_rm1 = mli_Image_idx(image, col - 1, row - 1);

                        idx_cp1_rp1 = mli_Image_idx(image, col + 1, row + 1);
                        idx_cp1_rp0 = mli_Image_idx(image, col + 1, row);
                        idx_cp1_rm1 = mli_Image_idx(image, col + 1, row - 1);

                        idx_cp0_rp1 = mli_Image_idx(image, col, row + 1);
                        idx_cp0_rm1 = mli_Image_idx(image, col, row - 1);

                        idx = mli_Image_idx(out, col, row);

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

                        out->raw[idx].r = mli_math_hypot(xr, yr);
                        out->raw[idx].g = mli_math_hypot(xg, yg);
                        out->raw[idx].b = mli_math_hypot(xb, yb);
                }
        }
}

void mli_Image_luminance_threshold_dilatation(
        const struct mli_Image *image,
        const float threshold,
        struct mli_Image *out)
{
        const int32_t rows = image->num_rows;
        const int32_t cols = image->num_cols;
        int32_t col, row;
        const struct mli_Color color_max = mli_Color_set(255., 255., 255.);
        for (row = 0; row < rows; row++) {
                for (col = 0; col < cols; col++) {
                        const struct mli_Color color_at =
                                mli_Image_at(image, col, row);
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
                                                        mli_Image_set(
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

void mli_Image_from_sum_and_exposure(
        const struct mli_Image *sum,
        const struct mli_Image *exposure,
        struct mli_Image *out)
{
        uint64_t pix;
        for (pix = 0u; pix < out->num_rows * out->num_cols; pix++) {
                out->raw[pix].r = sum->raw[pix].r / exposure->raw[pix].r;
                out->raw[pix].g = sum->raw[pix].g / exposure->raw[pix].g;
                out->raw[pix].b = sum->raw[pix].b / exposure->raw[pix].b;
        }
}

void mli_Pixels_set_all_from_image(
        struct mli_Pixels *pixels,
        const struct mli_Image *image)
{
        struct mli_PixelWalk walk =
                mli_PixelWalk_set(image->num_cols, image->num_rows, 16u);
        uint64_t i;
        const uint64_t num_pixel = image->num_rows * image->num_cols;
        assert(num_pixel == pixels->capacity);
        pixels->size = 0;
        for (i = 0; i < num_pixel; i++) {
                mli_Pixels_push_back(pixels, mli_PixelWalk_get(&walk));
                mli_PixelWalk_walk(&walk);
        }
}

void mli_Pixels_above_threshold(
        const struct mli_Image *image,
        const float threshold,
        struct mli_Pixels *pixels)
{
        struct mli_PixelWalk walk =
                mli_PixelWalk_set(image->num_cols, image->num_rows, 16u);
        uint64_t i;
        const uint64_t num_pixel = image->num_rows * image->num_cols;

        pixels->size = 0;
        for (i = 0; i < num_pixel; i++) {
                struct mli_Pixel px = mli_PixelWalk_get(&walk);
                double lum = 0.0;
                struct mli_Color c = mli_Image_at(image, px.col, px.row);
                lum = c.r + c.g + c.b;
                if (lum > threshold) {
                        mli_Pixels_push_back(pixels, px);
                }
                mli_PixelWalk_walk(&walk);
        }
}

void mli_Image_assign_pixel_colors_to_sum_and_exposure_image(
        const struct mli_Pixels *pixels,
        const struct mli_Image *colors,
        struct mli_Image *sum_image,
        struct mli_Image *exposure_image)
{
        uint64_t pix;
        for (pix = 0u; pix < pixels->size; pix++) {
                const struct mli_Pixel px = pixels->array[pix];
                const uint64_t idx = mli_Image_idx(sum_image, px.col, px.row);
                sum_image->raw[idx].r += colors->raw[idx].r;
                sum_image->raw[idx].g += colors->raw[idx].g;
                sum_image->raw[idx].b += colors->raw[idx].b;
                exposure_image->raw[idx].r += 1.;
                exposure_image->raw[idx].g += 1.;
                exposure_image->raw[idx].b += 1.;
        }
}

void mli_Image_copy(
        const struct mli_Image *source,
        struct mli_Image *destination)
{
        uint64_t pix;
        assert(source->num_rows == destination->num_rows);
        assert(source->num_cols == destination->num_cols);
        for (pix = 0u; pix < source->num_rows * source->num_cols; pix++) {
                destination->raw[pix] = source->raw[pix];
        }
}

void mli_Image_fabs_difference(
        const struct mli_Image *a,
        const struct mli_Image *b,
        struct mli_Image *out)
{
        uint64_t pix;
        assert(a->num_cols == b->num_cols);
        assert(a->num_rows == b->num_rows);
        assert(a->num_cols == out->num_cols);
        assert(a->num_rows == out->num_rows);
        for (pix = 0; pix < out->num_cols * out->num_rows; pix++) {
                out->raw[pix].r = fabs(a->raw[pix].r - b->raw[pix].r);
                out->raw[pix].g = fabs(a->raw[pix].g - b->raw[pix].g);
                out->raw[pix].b = fabs(a->raw[pix].b - b->raw[pix].b);
        }
}

void mli_Image_histogram(
        struct mli_Image *img,
        const double *col_bin_edges,
        const double *row_bin_edges,
        const double col_val,
        const double row_val,
        const struct mli_Color weight)
{
        uint64_t col_upper_idx, row_upper_idx;
        int valid_col, valid_row;

        col_upper_idx = MLI_MATH_UPPER_COMPARE_double(
                col_bin_edges, img->num_cols + 1, col_val);

        row_upper_idx = MLI_MATH_UPPER_COMPARE_double(
                row_bin_edges, img->num_rows + 1, row_val);

        valid_col = col_upper_idx > 0 && col_upper_idx < img->num_cols + 1;
        valid_row = row_upper_idx > 0 && row_upper_idx < img->num_rows + 1;

        if (valid_col && valid_row) {
                const uint32_t col_idx = col_upper_idx - 1;
                const uint32_t row_idx = row_upper_idx - 1;
                const uint64_t pix = mli_Image_idx(img, col_idx, row_idx);
                img->raw[pix] = mli_Color_add(img->raw[pix], weight);
        }
}

struct mli_Color mli_Image_max(const struct mli_Image *img)
{
        struct mli_Color max = mli_Color_set(-FLT_MAX, -FLT_MAX, -FLT_MAX);
        uint64_t pix;
        for (pix = 0u; pix < img->num_rows * img->num_cols; pix++) {
                struct mli_Color c = img->raw[pix];
                if (c.r > max.r) {
                        max.r = c.r;
                }
                if (c.g > max.g) {
                        max.g = c.g;
                }
                if (c.b > max.b) {
                        max.b = c.b;
                }
        }
        return max;
}

void mli_Image_multiply(struct mli_Image *img, const struct mli_Color color)
{
        uint64_t pix;
        for (pix = 0u; pix < img->num_rows * img->num_cols; pix++) {
                img->raw[pix].r *= color.r;
                img->raw[pix].g *= color.g;
                img->raw[pix].b *= color.b;
        }
}

void mli_Image_divide_pixelwise(
        const struct mli_Image *numerator,
        const struct mli_Image *denominator,
        struct mli_Image *out)
{
        uint64_t p;
        for (p = 0; p < out->num_rows * out->num_cols; p++) {
                out->raw[p].r = numerator->raw[p].r / denominator->raw[p].r;
                out->raw[p].g = numerator->raw[p].g / denominator->raw[p].g;
                out->raw[p].b = numerator->raw[p].b / denominator->raw[p].b;
        }
}
