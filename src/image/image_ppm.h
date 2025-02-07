/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_IMAGE_PPM_H_
#define MLI_IMAGE_PPM_H_

#include "../image/image.h"
#include "../chk/chk.h"
#include "../io/io.h"

enum mli_image_ppm_color_depth {
        MLI_IMAGE_PPM_COLOR_DEPTH_8BIT = 8,
        MLI_IMAGE_PPM_COLOR_DEPTH_16BIT = 16
};

chk_rc mli_Image_to_io(
        const struct mli_Image *img,
        struct mli_IO *f,
        const uint64_t color_depth_num_bit);
chk_rc mli_Image_from_io(struct mli_Image *img, struct mli_IO *f);

chk_rc mli_image_ppm__read_color_8bit(
        struct mli_Color *color,
        struct mli_IO *f);
chk_rc mli_image_ppm__write_color_8bit(
        const struct mli_Color color,
        struct mli_IO *f);

chk_rc mli_image_ppm__read_color_16bit(
        struct mli_Color *color,
        struct mli_IO *f);
chk_rc mli_image_ppm__write_color_16bit(
        const struct mli_Color color,
        struct mli_IO *f);

#endif
