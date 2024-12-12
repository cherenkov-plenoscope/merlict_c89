/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "../image/image.h"
#include <assert.h>
#include "../image/image_PixelWalk.h"
#include "../math/math.h"
#include "../chk/chk.h"
#include <float.h>

#define MLI_IMAGE_CHUNK_EDGE_SIZE (2u * 3u * 5u)

struct mli_Image mli_Image_init(void)
{
        struct mli_Image out;
        out.num_cols = 0u;
        out.num_rows = 0u;
        out.raw = NULL;

        out.geometry = mli_image_ChunkGeometry_set(0, 0, 1);
        out.chunks = NULL;
        return out;
}

void mli_Image_free(struct mli_Image *self)
{
        uint64_t crow, ccol;
        for (crow = 0; crow < self->geometry.num_chunks_row; crow++) {
                for (ccol = 0; ccol < self->geometry.num_chunks_col; ccol++) {
                        mli_image_Chunk_free(&self->chunks[crow][ccol]);
                }
                free(self->chunks[crow]);
        }
        free(self->chunks);

        free(self->raw);
        (*self) = mli_Image_init();
}

int mli_Image__malloc(
        struct mli_Image *self,
        const uint32_t num_cols,
        const uint32_t num_rows)
{
        uint64_t crow, ccol;
        mli_Image_free(self);

        self->geometry = mli_image_ChunkGeometry_set(
                num_cols, num_rows, MLI_IMAGE_CHUNK_EDGE_SIZE);

        chk_malloc(
                self->chunks,
                struct mli_image_Chunk *,
                self->geometry.num_chunks_row);

        for (crow = 0; crow < self->geometry.num_chunks_row; crow++) {
                chk_malloc(
                        self->chunks[crow],
                        struct mli_image_Chunk,
                        self->geometry.num_chunks_col);
                for (ccol = 0; ccol < self->geometry.num_chunks_col; ccol++) {
                        chk(mli_image_Chunk_malloc(
                                &self->chunks[crow][ccol],
                                self->geometry.chunk_edge_size));
                }
        }

        self->num_cols = num_cols;
        self->num_rows = num_rows;
        chk_malloc(
                self->raw, struct mli_Color, self->num_cols * self->num_rows);
        return 1;
chk_error:
        mli_Image_free(self);
        return 0;
}

int mli_Image_malloc(
        struct mli_Image *self,
        const uint32_t num_cols,
        const uint32_t num_rows)
{
        if (self->geometry.num_cols == num_cols &&
            self->geometry.num_rows == num_rows) {
                return 1;
        } else {
                return mli_Image__malloc(self, num_cols, num_rows);
        }
}

uint64_t mli_Image_num_pixel(const struct mli_Image *self)
{
        return self->geometry.num_cols * self->geometry.num_rows;
}

/* SET GET PIXELWALK */

void mli_Image__set_by_PixelWalk(
        const struct mli_Image *img,
        const struct mli_image_PixelWalk walk,
        const struct mli_Color color)
{
        mli_image_Chunk_set(
                &img->chunks[walk.chunk_row][walk.chunk_col],
                walk.sub_col,
                walk.sub_row,
                color);
}

struct mli_Color mli_Image__get_by_PixelWalk(
        const struct mli_Image *img,
        const struct mli_image_PixelWalk walk)
{
        return mli_image_Chunk_get(
                &img->chunks[walk.chunk_row][walk.chunk_col],
                walk.sub_col,
                walk.sub_row);
}

/* SET GET PIXEL */

struct mli_Color mli_Image_get_by_Pixel(
        const struct mli_Image *self,
        const struct mli_image_Pixel px)
{
        struct mli_image_PixelWalk w =
                mli_image_PixelWalk_from_pixel(&self->geometry, px);
        return mli_Image__get_by_PixelWalk(self, w);
}

void mli_Image_set_by_Pixel(
        struct mli_Image *self,
        const struct mli_image_Pixel px,
        const struct mli_Color color)
{
        struct mli_image_PixelWalk w =
                mli_image_PixelWalk_from_pixel(&self->geometry, px);
        mli_Image__set_by_PixelWalk(self, w, color);
}

/* SET GET ROW COL */

void mli_Image_set(
        struct mli_Image *img,
        const uint32_t col,
        const uint32_t row,
        const struct mli_Color color)
{
        /* chunk */
        struct mli_image_Pixel px;
        struct mli_image_PixelWalk w;
        px.col = col;
        px.row = row;
        w = mli_image_PixelWalk_from_pixel(&img->geometry, px);
        mli_Image__set_by_PixelWalk(img, w, color);

        /* flat */
        img->raw[mli_Image_idx(img, col, row)] = color;
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

/* flat */
uint32_t mli_Image_idx(
        const struct mli_Image *img,
        const uint32_t col,
        const uint32_t row)
{
        return col * img->num_rows + row;
}



void mli_Image_set_all_pixel(
        const struct mli_Image *self,
        const struct mli_Color color)
{
        uint64_t row, col;

        uint64_t i;
        const uint64_t NUM = mli_Image_num_pixel(self);
        struct mli_image_PixelWalk w = mli_image_PixelWalk_init();
        for (i = 0; i < NUM; i++) {
                mli_Image__set_by_PixelWalk(self, w, color);
                mli_image_PixelWalk_walk(&w, &self->geometry);
        }

        /* flat */
        for (row = 0; row < self->num_rows; row++) {
                for (col = 0; col < self->num_cols; col++) {
                        self->raw[mli_Image_idx(self, col, row)] = color;
                }
        }
}

int mli_Image_scale_down_twice(
        const struct mli_Image *src,
        struct mli_Image *dst)
{
        uint64_t row, col, sr, sc, i;
        uint64_t DST_NUM = mli_Image_num_pixel(dst);
        struct mli_image_PixelWalk w = mli_image_PixelWalk_init();
        chk_msg(dst->num_cols * 2u == src->num_cols,
                "Expected dst.num_cols*2u == src.num_cols");
        chk_msg(dst->num_rows * 2u == src->num_rows,
                "Expected dst.num_rows*2u == src.num_rows");

        for (i = 0; i < DST_NUM; i++) {
                struct mli_Color mix[4];
                const struct mli_image_Pixel px =
                        mli_image_PixelWalk_get_Pixel(&w, &dst->geometry);
                sr = px.row * 2u;
                sc = px.col * 2u;

                mix[0] = mli_Image_at(src, sc + 0, sr + 0);
                mix[1] = mli_Image_at(src, sc + 0, sr + 1);
                mix[2] = mli_Image_at(src, sc + 1, sr + 0);
                mix[3] = mli_Image_at(src, sc + 1, sr + 1);

                mli_Image__set_by_PixelWalk(dst, w, mli_Color_mean(mix, 4));
                mli_image_PixelWalk_walk(&w, &dst->geometry);
        }

        /* flat */
        for (row = 0; row < dst->num_rows; row++) {
                for (col = 0; col < dst->num_cols; col++) {
                        struct mli_Color mix[4];
                        sr = row * 2u;
                        sc = col * 2u;
                        mix[0] = mli_Image_at(src, sc + 0, sr + 0);
                        mix[1] = mli_Image_at(src, sc + 0, sr + 1);
                        mix[2] = mli_Image_at(src, sc + 1, sr + 0);
                        mix[3] = mli_Image_at(src, sc + 1, sr + 1);
                        mli_Image_set(dst, col, row, mli_Color_mean(mix, 4));
                }
        }
        return 1;
chk_error:
        return 0;
}

int mli_Image_sobel_chunk(const struct mli_Image *src, struct mli_Image *dst)
{
        uint64_t i;
        uint64_t NUM = mli_Image_num_pixel(src);

        uint16_t row, col;
        struct mli_image_Pixel c_0_r_0;
        struct mli_image_Pixel cm1_rp1;
        struct mli_image_Pixel cm1_r_0;
        struct mli_image_Pixel cm1_rm1;
        struct mli_image_Pixel cp1_rp1;
        struct mli_image_Pixel cp1_r_0;
        struct mli_image_Pixel cp1_rm1;
        struct mli_image_Pixel c_0_rp1;
        struct mli_image_Pixel c_0_rm1;

        struct mli_image_PixelWalk w = mli_image_PixelWalk_init();

        struct mli_Color x, y, color_result;

        struct mli_Color C_cm1_rp1;
        struct mli_Color C_cm1_r_0;
        struct mli_Color C_cm1_rm1;

        struct mli_Color C_c_0_rp1;
        /*               C_c_0_r_0 */
        struct mli_Color C_c_0_rm1;

        struct mli_Color C_cp1_rp1;
        struct mli_Color C_cp1_r_0;
        struct mli_Color C_cp1_rm1;

        /*
         *       ______ ______ ______
         *      | c:-1 | c: 0 | c:+1 |
         *      | r:+1 | r:+1 | r:+1 |
         *      |______|______|______|
         *      | c:-1 | c: 0 | c:+1 |
         *      | r: 0 | r: 0 | r: 0 |
         *      |______|______|______|
         *      | c:-1 | c: 0 | c:+1 |
         *      | r:-1 | r:-1 | r:-1 |
         *      |______|______|______|
         */

        chk(mli_Image_malloc(
                dst, src->geometry.num_cols, src->geometry.num_rows));

        for (i = 0; i < NUM; i++) {
                c_0_r_0 = mli_image_PixelWalk_get_Pixel(&w, &src->geometry);
                row = c_0_r_0.row;
                col = c_0_r_0.col;

                cm1_rp1 = mli_image_Pixel_set(col - 1, row + 1);
                cm1_r_0 = mli_image_Pixel_set(col - 1, row);
                cm1_rm1 = mli_image_Pixel_set(col - 1, row - 1);

                c_0_rp1 = mli_image_Pixel_set(col, row + 1);
                /* c_0_r_0 */
                c_0_rm1 = mli_image_Pixel_set(col, row - 1);

                cp1_rp1 = mli_image_Pixel_set(col + 1, row + 1);
                cp1_r_0 = mli_image_Pixel_set(col + 1, row);
                cp1_rm1 = mli_image_Pixel_set(col + 1, row - 1);

                x = mli_Color_set(0., 0., 0.);
                y = mli_Color_set(0., 0., 0.);

                C_cm1_rp1 = mli_Image_get_by_Pixel(src, cm1_rp1);
                C_cm1_r_0 = mli_Image_get_by_Pixel(src, cm1_r_0);
                C_cm1_rm1 = mli_Image_get_by_Pixel(src, cm1_rm1);

                C_c_0_rp1 = mli_Image_get_by_Pixel(src, c_0_rp1);

                C_c_0_rm1 = mli_Image_get_by_Pixel(src, c_0_rm1);

                C_cp1_rp1 = mli_Image_get_by_Pixel(src, cp1_rp1);
                C_cp1_r_0 = mli_Image_get_by_Pixel(src, cp1_r_0);
                C_cp1_rm1 = mli_Image_get_by_Pixel(src, cp1_rm1);

                x = mli_Color_add(x, mli_Color_multiply(C_cm1_rp1, -1.0));
                x = mli_Color_add(x, mli_Color_multiply(C_cm1_r_0, -2.0));
                x = mli_Color_add(x, mli_Color_multiply(C_cm1_rm1, -1.0));

                x = mli_Color_add(x, mli_Color_multiply(C_cp1_rp1, +1.0));
                x = mli_Color_add(x, mli_Color_multiply(C_cp1_r_0, +2.0));
                x = mli_Color_add(x, mli_Color_multiply(C_cp1_rm1, +1.0));

                y = mli_Color_add(x, mli_Color_multiply(C_cm1_rp1, -1.0));
                y = mli_Color_add(x, mli_Color_multiply(C_c_0_rp1, -2.0));
                y = mli_Color_add(x, mli_Color_multiply(C_cp1_rp1, -1.0));

                y = mli_Color_add(x, mli_Color_multiply(C_cm1_rm1, +1.0));
                y = mli_Color_add(x, mli_Color_multiply(C_c_0_rm1, +2.0));
                y = mli_Color_add(x, mli_Color_multiply(C_cp1_rm1, +1.0));

                color_result = mli_Color_set(
                        mli_math_hypot(x.r, y.r),
                        mli_math_hypot(x.g, y.g),
                        mli_math_hypot(x.b, y.b));
                mli_Image__set_by_PixelWalk(dst, w, color_result);
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

void mli_image_PixelVector_set_all_from_image(
        struct mli_image_PixelVector *pixels,
        const struct mli_Image *image)
{
        struct mli_image_PixelWalk walk = mli_image_PixelWalk_init();
        uint64_t i;
        const uint64_t num_pixel = image->num_rows * image->num_cols;
        assert(num_pixel == pixels->capacity);
        pixels->size = 0;
        for (i = 0; i < num_pixel; i++) {
                struct mli_image_Pixel px =
                        mli_image_PixelWalk_get_Pixel(&walk, &image->geometry);
                mli_image_PixelVector_push_back(pixels, px);
                mli_image_PixelWalk_walk(&walk, &image->geometry);
        }
}

void mli_image_PixelVector_above_threshold(
        const struct mli_Image *image,
        const float threshold,
        struct mli_image_PixelVector *pixels)
{
        struct mli_image_PixelWalk walk = mli_image_PixelWalk_init();
        uint64_t i;
        const uint64_t num_pixel = image->num_rows * image->num_cols;

        pixels->size = 0;
        for (i = 0; i < num_pixel; i++) {
                struct mli_image_Pixel px =
                        mli_image_PixelWalk_get_Pixel(&walk, &image->geometry);
                double lum = 0.0;
                struct mli_Color c = mli_Image_at(image, px.col, px.row);
                lum = c.r + c.g + c.b;
                if (lum > threshold) {
                        mli_image_PixelVector_push_back(pixels, px);
                }
                mli_image_PixelWalk_walk(&walk, &image->geometry);
        }
}

void mli_Image_assign_pixel_colors_to_sum_and_exposure_image(
        const struct mli_image_PixelVector *pixels,
        const struct mli_Image *colors,
        struct mli_Image *sum_image,
        struct mli_Image *exposure_image)
{
        uint64_t pix;
        for (pix = 0u; pix < pixels->size; pix++) {
                const struct mli_image_Pixel px = pixels->array[pix];
                const uint64_t idx = mli_Image_idx(sum_image, px.col, px.row);
                sum_image->raw[idx].r += colors->raw[idx].r;
                sum_image->raw[idx].g += colors->raw[idx].g;
                sum_image->raw[idx].b += colors->raw[idx].b;
                exposure_image->raw[idx].r += 1.;
                exposure_image->raw[idx].g += 1.;
                exposure_image->raw[idx].b += 1.;
        }
}

int mli_Image_copy(const struct mli_Image *src, struct mli_Image *dst)
{
        uint64_t i;
        const uint64_t NUM = mli_Image_num_pixel(src);
        struct mli_image_PixelWalk walk = mli_image_PixelWalk_init();
        struct mli_Color color;

        chk(mli_Image_malloc(
                dst, src->geometry.num_cols, src->geometry.num_rows));

        for (i = 0u; i < NUM; i++) {
                color = mli_Image__get_by_PixelWalk(src, walk);
                mli_Image__set_by_PixelWalk(dst, walk, color);
                mli_image_PixelWalk_walk(&walk, &src->geometry);
        }

        /* flat */
        for (i = 0u; i < NUM; i++) {
                dst->raw[i] = src->raw[i];
        }

        return 1;
chk_error:
        return 0;
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
