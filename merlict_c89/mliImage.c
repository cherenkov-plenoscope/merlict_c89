/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliImage.h"

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

void mliImage_set_all_pixel(
        const struct mliImage *img,
        const struct mliColor color)
{
        uint64_t row, col;
        for (row = 0; row < img->num_rows; row++) {
                for (col = 0; col < img->num_cols; col++) {
                        img->raw[mliImage_idx(img, col, row)] = color;
                }
        }
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

void mliPixels_set_all_from_image(
        struct mliPixels *pixels,
        const struct mliImage *image)
{
        uint64_t i, r, c;
        assert(image->num_cols * image->num_rows == pixels->num_pixels);
        i = 0u;
        for (c = 0u; c < image->num_cols; c++) {
                for (r = 0u; r < image->num_rows; r++) {
                        pixels->pixels[i].row = r;
                        pixels->pixels[i].col = c;
                        i++;
                }
        }
}

void mliPixels_above_threshold(
        const struct mliImage *to_do_image,
        const float threshold,
        struct mliPixels *pixels)
{
        uint64_t row, col;
        pixels->num_pixels_to_do = 0;
        for (row = 0; row < to_do_image->num_rows; row++) {
                for (col = 0; col < to_do_image->num_cols; col++) {
                        double lum = 0.0;
                        struct mliColor c = mliImage_at(to_do_image, col, row);
                        lum = c.r + c.g + c.b;
                        if (lum > threshold) {
                                pixels->pixels[pixels->num_pixels_to_do].row =
                                        row;
                                pixels->pixels[pixels->num_pixels_to_do].col =
                                        col;
                                pixels->num_pixels_to_do += 1;
                        }
                }
        }
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

void mliImage_copy(const struct mliImage *source, struct mliImage *destination)
{
        uint64_t pix;
        assert(source->num_rows == destination->num_rows);
        assert(source->num_cols == destination->num_cols);
        for (pix = 0u; pix < source->num_rows * source->num_cols; pix++) {
                destination->raw[pix] = source->raw[pix];
        }
}

void mliImage_fabs_difference(
        const struct mliImage *a,
        const struct mliImage *b,
        struct mliImage *out)
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

void mliImage_histogram(
        struct mliImage *img,
        const double *col_bin_edges,
        const double *row_bin_edges,
        const double col_val,
        const double row_val,
        const struct mliColor weight)
{
        uint64_t col_upper_idx, row_upper_idx;
        int valid_col, valid_row;

        col_upper_idx = mli_upper_compare_double(
                col_bin_edges, img->num_cols + 1, col_val);

        row_upper_idx = mli_upper_compare_double(
                row_bin_edges, img->num_rows + 1, row_val);

        valid_col = col_upper_idx > 0 && col_upper_idx < img->num_cols + 1;
        valid_row = row_upper_idx > 0 && row_upper_idx < img->num_rows + 1;

        if (valid_col && valid_row) {
                const uint32_t col_idx = col_upper_idx - 1;
                const uint32_t row_idx = row_upper_idx - 1;
                const uint64_t pix = mliImage_idx(img, col_idx, row_idx);
                img->raw[pix] = mliColor_add(img->raw[pix], weight);
        }
}

struct mliColor mliImage_max(const struct mliImage *img)
{
        struct mliColor max = mliColor_set(-FLT_MAX, -FLT_MAX, -FLT_MAX);
        uint64_t pix;
        for (pix = 0u; pix < img->num_rows * img->num_cols; pix++) {
                struct mliColor c = img->raw[pix];
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

void mliImage_multiply(struct mliImage *img, const struct mliColor color)
{
        uint64_t pix;
        for (pix = 0u; pix < img->num_rows * img->num_cols; pix++) {
                img->raw[pix].r *= color.r;
                img->raw[pix].g *= color.g;
                img->raw[pix].b *= color.b;
        }
}
