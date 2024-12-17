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

int mli_Image_malloc_same_size(
        struct mli_Image *self,
        const struct mli_Image *other)
{
        return mli_Image_malloc(
                self, mli_Image_num_cols(other), mli_Image_num_rows(other));
}

uint64_t mli_Image_num_pixel(const struct mli_Image *self)
{
        return self->geometry.num_cols * self->geometry.num_rows;
}

uint64_t mli_Image_num_cols(const struct mli_Image *self)
{
        return self->geometry.num_cols;
}

uint64_t mli_Image_num_rows(const struct mli_Image *self)
{
        return self->geometry.num_rows;
}

/* SET GET PIXELWALK */

void mli_Image__set_by_PixelWalk(
        const struct mli_Image *self,
        const struct mli_image_PixelWalk walk,
        const struct mli_Color color)
{
        mli_image_Chunk_set(
                &self->chunks[walk.chunk_row][walk.chunk_col],
                walk.sub_col,
                walk.sub_row,
                color);
}

struct mli_Color mli_Image__get_by_PixelWalk(
        const struct mli_Image *self,
        const struct mli_image_PixelWalk walk)
{
        return mli_image_Chunk_get(
                &self->chunks[walk.chunk_row][walk.chunk_col],
                walk.sub_col,
                walk.sub_row);
}

struct mli_Color *mli_Image__get_ptr_by_PixelWalk(
        const struct mli_Image *self,
        const struct mli_image_PixelWalk walk)
{
        return mli_image_Chunk_get_ptr(
                &self->chunks[walk.chunk_row][walk.chunk_col],
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

void mli_Image_set_by_col_row(
        struct mli_Image *self,
        const uint32_t col,
        const uint32_t row,
        const struct mli_Color color)
{
        struct mli_image_Pixel px;
        px.col = col;
        px.row = row;
        mli_Image_set_by_Pixel(self, px, color);
}

struct mli_Color mli_Image_get_by_col_row(
        const struct mli_Image *self,
        const uint32_t col,
        const uint32_t row)
{
        struct mli_image_Pixel px;
        px.col = col;
        px.row = row;
        return mli_Image_get_by_Pixel(self, px);
}

void mli_Image_set_all(
        const struct mli_Image *self,
        const struct mli_Color color)
{
        uint64_t i;
        const uint64_t NUM = mli_Image_num_pixel(self);
        struct mli_image_PixelWalk w = mli_image_PixelWalk_init();
        for (i = 0; i < NUM; i++) {
                mli_Image__set_by_PixelWalk(self, w, color);
                mli_image_PixelWalk_walk(&w, &self->geometry);
        }
}

int mli_Image_scale_down_twice(
        const struct mli_Image *src,
        struct mli_Image *dst)
{
        uint64_t sr, sc, i;
        uint64_t DST_NUM = mli_Image_num_pixel(dst);
        struct mli_image_PixelWalk w = mli_image_PixelWalk_init();

        chk_msg(mli_Image_num_cols(src) % 2 == 0,
                "Expected num_cols in src image to be exact multiple of 2.");
        chk_msg(mli_Image_num_rows(src) % 2 == 0,
                "Expected num_rows in src image to be exact multiple of 2.");
        chk(mli_Image_malloc(
                dst, mli_Image_num_cols(src) / 2, mli_Image_num_rows(src) / 2));

        for (i = 0; i < DST_NUM; i++) {
                struct mli_Color mix[4];
                const struct mli_image_Pixel px =
                        mli_image_PixelWalk_get_Pixel(&w, &dst->geometry);
                sr = px.row * 2u;
                sc = px.col * 2u;

                mix[0] = mli_Image_get_by_col_row(src, sc + 0, sr + 0);
                mix[1] = mli_Image_get_by_col_row(src, sc + 0, sr + 1);
                mix[2] = mli_Image_get_by_col_row(src, sc + 1, sr + 0);
                mix[3] = mli_Image_get_by_col_row(src, sc + 1, sr + 1);

                mli_Image__set_by_PixelWalk(dst, w, mli_Color_mean(mix, 4));
                mli_image_PixelWalk_walk(&w, &dst->geometry);
        }

        return 1;
chk_error:
        return 0;
}

int mli_Image_sobel(const struct mli_Image *src, struct mli_Image *dst)
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
         *       relative coordinates of folding kernel
         *       'c' is column and 'r' is row.
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

        /*
         *       weights for x-gradient-kernel
         *       ______ ______ ______
         *      | w:-1 | w: 0 | w:+1 |
         *      |      |      |      |
         *      |______|______|______|
         *      | w:-2 | w: 0 | w:+2 |
         *      |      |      |      |
         *      |______|______|______|
         *      | w:-1 | w: 0 | w:+1 |
         *      |      |      |      |
         *      |______|______|______|
         */

        /*
         *       weights for y-gradient-kernel
         *       ______ ______ ______
         *      | w:+1 | w:+2 | w:+1 |
         *      |      |      |      |
         *      |______|______|______|
         *      | w: 0 | w: 0 | w: 0 |
         *      |      |      |      |
         *      |______|______|______|
         *      | w:-1 | w:-2 | w:-1 |
         *      |      |      |      |
         *      |______|______|______|
         */

        chk(mli_Image_malloc_same_size(dst, src));
        for (i = 0; i < NUM; i++) {
                int is_on_edge = 0;
                c_0_r_0 = mli_image_PixelWalk_get_Pixel(&w, &src->geometry);
                row = c_0_r_0.row;
                col = c_0_r_0.col;
                if (row == 0 || row == mli_Image_num_rows(src) - 1)
                        is_on_edge = 1;
                if (col == 0 || col == mli_Image_num_cols(src) - 1)
                        is_on_edge = 1;

                if (is_on_edge) {
                        color_result = mli_Color_set(0.0, 0.0, 0.0);
                } else {
                        cm1_rp1 = mli_image_Pixel_set_col_row(col - 1, row + 1);
                        cm1_r_0 = mli_image_Pixel_set_col_row(col - 1, row + 0);
                        cm1_rm1 = mli_image_Pixel_set_col_row(col - 1, row - 1);

                        c_0_rp1 = mli_image_Pixel_set_col_row(col, row + 1);
                        /* c_0_r_0 */
                        c_0_rm1 = mli_image_Pixel_set_col_row(col, row - 1);

                        cp1_rp1 = mli_image_Pixel_set_col_row(col + 1, row + 1);
                        cp1_r_0 = mli_image_Pixel_set_col_row(col + 1, row + 0);
                        cp1_rm1 = mli_image_Pixel_set_col_row(col + 1, row - 1);

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

                        x = mli_Color_add(
                                x, mli_Color_multiply(C_cm1_rp1, -1.0));
                        x = mli_Color_add(
                                x, mli_Color_multiply(C_cm1_r_0, -2.0));
                        x = mli_Color_add(
                                x, mli_Color_multiply(C_cm1_rm1, -1.0));

                        x = mli_Color_add(
                                x, mli_Color_multiply(C_cp1_rp1, +1.0));
                        x = mli_Color_add(
                                x, mli_Color_multiply(C_cp1_r_0, +2.0));
                        x = mli_Color_add(
                                x, mli_Color_multiply(C_cp1_rm1, +1.0));

                        y = mli_Color_add(
                                y, mli_Color_multiply(C_cm1_rp1, -1.0));
                        y = mli_Color_add(
                                y, mli_Color_multiply(C_c_0_rp1, -2.0));
                        y = mli_Color_add(
                                y, mli_Color_multiply(C_cp1_rp1, -1.0));

                        y = mli_Color_add(
                                y, mli_Color_multiply(C_cm1_rm1, +1.0));
                        y = mli_Color_add(
                                y, mli_Color_multiply(C_c_0_rm1, +2.0));
                        y = mli_Color_add(
                                y, mli_Color_multiply(C_cp1_rm1, +1.0));

                        color_result = mli_Color_set(
                                mli_math_hypot(x.r, y.r),
                                mli_math_hypot(x.g, y.g),
                                mli_math_hypot(x.b, y.b));
                }
                mli_Image__set_by_PixelWalk(dst, w, color_result);
                mli_image_PixelWalk_walk(&w, &src->geometry);
        }
        return 1;
chk_error:
        return 0;
}

int mli_Image_luminance_threshold_dilatation(
        const struct mli_Image *self,
        const float threshold,
        const struct mli_Color marker,
        struct mli_Image *out)
{
        uint64_t i;
        const uint64_t NUM = mli_Image_num_pixel(self);
        struct mli_image_PixelWalk w = mli_image_PixelWalk_init();
        const int32_t num_rows = mli_Image_num_rows(self);
        const int32_t num_cols = mli_Image_num_cols(self);

        chk(mli_Image_malloc_same_size(out, self));

        for (i = 0u; i < NUM; i++) {
                const struct mli_Color c = mli_Image__get_by_PixelWalk(self, w);
                const float luminance = mli_Color_luminance(c);
                struct mli_image_Pixel px =
                        mli_image_PixelWalk_get_Pixel(&w, &self->geometry);
                if (luminance > threshold) {
                        int32_t orow, ocol;
                        for (orow = -1; orow < 2; orow++) {
                                for (ocol = -1; ocol < 2; ocol++) {
                                        if (px.row + orow >= 0 &&
                                            px.col + ocol >= 0 &&
                                            px.row + orow < num_rows &&
                                            px.col + ocol < num_cols) {
                                                mli_Image_set_by_col_row(
                                                        out,
                                                        px.col + ocol,
                                                        px.row + orow,
                                                        marker);
                                        }
                                }
                        }
                }
        }
        return 1;
chk_error:
        return 0;
}

void mli_image_PixelVector_push_back_all_from_image(
        struct mli_image_PixelVector *pixels,
        const struct mli_Image *image)
{
        uint64_t i;
        const uint64_t NUM = mli_Image_num_pixel(image);
        struct mli_image_PixelWalk w = mli_image_PixelWalk_init();

        pixels->size = 0;
        for (i = 0u; i < NUM; i++) {
                struct mli_image_Pixel px =
                        mli_image_PixelWalk_get_Pixel(&w, &image->geometry);
                mli_image_PixelVector_push_back(pixels, px);
                mli_image_PixelWalk_walk(&w, &image->geometry);
        }
}

int mli_image_PixelVector_above_threshold(
        const struct mli_Image *self,
        const float threshold,
        struct mli_image_PixelVector *pixels)
{
        uint64_t i;
        const uint64_t NUM = mli_Image_num_pixel(self);
        struct mli_image_PixelWalk w = mli_image_PixelWalk_init();

        pixels->size = 0;
        for (i = 0u; i < NUM; i++) {
                struct mli_image_Pixel px =
                        mli_image_PixelWalk_get_Pixel(&w, &self->geometry);
                double lum = 0.0;
                struct mli_Color c = mli_Image__get_by_PixelWalk(self, w);
                lum = c.r + c.g + c.b;
                if (lum > threshold) {
                        chk(mli_image_PixelVector_push_back(pixels, px));
                }
                mli_image_PixelWalk_walk(&w, &self->geometry);
        }
        return 1;
chk_error:
        return 0;
}

int mli_Image_copy(const struct mli_Image *src, struct mli_Image *dst)
{
        uint64_t i;
        const uint64_t NUM = mli_Image_num_pixel(src);
        struct mli_image_PixelWalk walk = mli_image_PixelWalk_init();
        struct mli_Color color;

        chk(mli_Image_malloc_same_size(dst, src));

        for (i = 0u; i < NUM; i++) {
                color = mli_Image__get_by_PixelWalk(src, walk);
                mli_Image__set_by_PixelWalk(dst, walk, color);
                mli_image_PixelWalk_walk(&walk, &src->geometry);
        }

        return 1;
chk_error:
        return 0;
}

int mli_Image_fabs_difference(
        const struct mli_Image *a,
        const struct mli_Image *b,
        struct mli_Image *out)
{
        uint64_t i;
        uint64_t NUM = mli_Image_num_pixel(a);
        struct mli_image_PixelWalk w = mli_image_PixelWalk_init();

        chk_msg(mli_image_ChunkGeometry_equal(a->geometry, b->geometry),
                "Expected images 'a' and 'b' to have the same geometry.");
        chk(mli_Image_malloc_same_size(out, a));

        for (i = 0; i < NUM; i++) {
                const struct mli_Color _a = mli_Image__get_by_PixelWalk(a, w);
                const struct mli_Color _b = mli_Image__get_by_PixelWalk(b, w);
                struct mli_Color _o;
                _o.r = fabs(_a.r - _b.r);
                _o.g = fabs(_a.g - _b.g);
                _o.b = fabs(_a.b - _b.b);
                mli_Image__set_by_PixelWalk(out, w, _o);
                mli_image_PixelWalk_walk(&w, &out->geometry);
        }
        return 1;
chk_error:
        return 0;
}

void mli_Image_histogram(
        struct mli_Image *self,
        const double *col_bin_edges,
        const double *row_bin_edges,
        const double col_val,
        const double row_val,
        const struct mli_Color weight)
{
        uint64_t col_upper_idx, row_upper_idx;
        int valid_col, valid_row;

        col_upper_idx = mli_math_upper_compare_double(
                col_bin_edges, mli_Image_num_cols(self) + 1, col_val);

        row_upper_idx = mli_math_upper_compare_double(
                row_bin_edges, mli_Image_num_rows(self) + 1, row_val);

        valid_col = col_upper_idx > 0 &&
                    col_upper_idx < mli_Image_num_cols(self) + 1;
        valid_row = row_upper_idx > 0 &&
                    row_upper_idx < mli_Image_num_rows(self) + 1;

        if (valid_col && valid_row) {
                const uint32_t col = col_upper_idx - 1;
                const uint32_t row = row_upper_idx - 1;
                const struct mli_image_PixelWalk w =
                        mli_image_PixelWalk_from_pixel(
                                &self->geometry,
                                mli_image_Pixel_set_col_row(col, row));
                struct mli_Color *c = mli_Image__get_ptr_by_PixelWalk(self, w);
                (*c) = mli_Color_add((*c), weight);
        }
}

struct mli_Color mli_Image_max(const struct mli_Image *self)
{
        struct mli_Color max = mli_Color_set(-FLT_MAX, -FLT_MAX, -FLT_MAX);
        uint64_t i;
        uint64_t NUM = mli_Image_num_pixel(self);
        struct mli_image_PixelWalk w = mli_image_PixelWalk_init();

        for (i = 0; i < NUM; i++) {
                struct mli_Color c = mli_Image__get_by_PixelWalk(self, w);
                if (c.r > max.r) {
                        max.r = c.r;
                }
                if (c.g > max.g) {
                        max.g = c.g;
                }
                if (c.b > max.b) {
                        max.b = c.b;
                }
                mli_image_PixelWalk_walk(&w, &self->geometry);
        }
        return max;
}

void mli_Image_multiply(struct mli_Image *self, const struct mli_Color color)
{
        uint64_t i;
        uint64_t NUM = mli_Image_num_pixel(self);
        struct mli_image_PixelWalk w = mli_image_PixelWalk_init();

        for (i = 0; i < NUM; i++) {
                struct mli_Color *_self =
                        mli_Image__get_ptr_by_PixelWalk(self, w);
                _self->r *= color.r;
                _self->g *= color.g;
                _self->b *= color.b;
                mli_image_PixelWalk_walk(&w, &self->geometry);
        }
}

void mli_Image_power(struct mli_Image *self, const struct mli_Color power)
{
        uint64_t i;
        uint64_t NUM = mli_Image_num_pixel(self);
        struct mli_image_PixelWalk w = mli_image_PixelWalk_init();

        for (i = 0; i < NUM; i++) {
                struct mli_Color *_self =
                        mli_Image__get_ptr_by_PixelWalk(self, w);
                _self->r = pow(_self->r, power.r);
                _self->g = pow(_self->g, power.g);
                _self->b = pow(_self->b, power.b);
                mli_image_PixelWalk_walk(&w, &self->geometry);
        }
}

int mli_Image_divide_pixelwise(
        const struct mli_Image *numerator,
        const struct mli_Image *denominator,
        struct mli_Image *out)
{
        uint64_t i;
        uint64_t NUM = mli_Image_num_pixel(out);
        struct mli_image_PixelWalk w = mli_image_PixelWalk_init();

        chk_msg(mli_image_ChunkGeometry_equal(
                        numerator->geometry, denominator->geometry),
                "Expected images 'numerator' and 'denominator' to have "
                "the same geometry.");
        chk(mli_Image_malloc_same_size(out, numerator));

        for (i = 0; i < NUM; i++) {
                const struct mli_Color _numerator =
                        mli_Image__get_by_PixelWalk(numerator, w);
                const struct mli_Color _denominator =
                        mli_Image__get_by_PixelWalk(denominator, w);
                struct mli_Color _out = mli_Color_set(
                        _numerator.r / _denominator.r,
                        _numerator.g / _denominator.g,
                        _numerator.b / _denominator.b);
                mli_Image__set_by_PixelWalk(out, w, _out);
                mli_image_PixelWalk_walk(&w, &out->geometry);
        }
        return 1;
chk_error:
        return 0;
}
