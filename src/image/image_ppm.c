/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "image_ppm.h"
#include "../version/version.h"
#include "../chk/chk.h"
#include "../string/string_numbers.h"
#include "../io/io_text.h"

int mli_Image_from_io(struct mli_Image *img, struct mli_IO *f)
{
        struct mli_String line = mli_String_init();
        uint64_t num_comment_lines = 0;
        uint64_t num_cols;
        uint64_t num_rows;
        uint64_t col;
        uint64_t row;
        chk_msg(mli_IO_text_read_line(f, &line, '\n'),
                "Can't read header-line.");
        chk_msg(mli_String_equal_cstr(&line, "P6"),
                "Expected starts with 'P6'.");

        while (1) {
                chk_msg(num_comment_lines < 1024,
                        "Expected < 1024 comment lines.");
                chk_msg(mli_IO_text_read_line(f, &line, '\n'),
                        "Can't read header-line.");
                if (mli_String_starts_with_cstr(&line, "#")) {
                        num_comment_lines += 1u;
                } else {
                        break;
                }
        }
        chk_msg(mli_String_to_uint64(&num_cols, &line, 10),
                "Can't parse num. columns.");
        chk_msg(mli_IO_text_read_line(f, &line, '\n'),
                "Can't read header-line.");
        chk_msg(mli_String_to_uint64(&num_rows, &line, 10),
                "Can't parse num. rows.");
        chk_msg(mli_IO_text_read_line(f, &line, '\n'),
                "Can't read header-line.");
        chk_msg(mli_String_equal_cstr(&line, "255"),
                "Expected 8bit range '255'.");

        chk_mem(mli_Image_malloc(img, num_cols, num_rows));
        for (row = 0; row < mli_Image_num_rows(img); row++) {
                for (col = 0; col < mli_Image_num_cols(img); col++) {
                        uint8_t r, g, b;
                        struct mli_Color color;
                        chk_IO_read(&r, sizeof(uint8_t), 1u, f);
                        chk_IO_read(&g, sizeof(uint8_t), 1u, f);
                        chk_IO_read(&b, sizeof(uint8_t), 1u, f);
                        color.r = (float)r;
                        color.g = (float)g;
                        color.b = (float)b;
                        mli_Image_set_by_col_row(img, col, row, color);
                }
        }
        mli_String_free(&line);
        return 1;
chk_error:
        mli_String_free(&line);
        mli_Image_free(img);
        return 0;
}

int mli_Image_to_io(const struct mli_Image *img, struct mli_IO *f)
{

        uint32_t col;
        uint32_t row;
        chk(mli_IO_text_write_cstr(f, "P6\n"));
        chk(mli_IO_text_write_cstr(f, "# merlict_c89\n"));
        chk(mli_IO_text_write_cstr_format(
                f,
                "# MLI_VERSION %d.%d.%d\n",
                MLI_VERSION_MAYOR,
                MLI_VERSION_MINOR,
                MLI_VERSION_PATCH));
        chk(mli_IO_text_write_cstr_format(f, "%ld\n", mli_Image_num_cols(img)));
        chk(mli_IO_text_write_cstr_format(f, "%ld\n", mli_Image_num_rows(img)));
        chk(mli_IO_text_write_cstr(f, "255\n"));
        for (row = 0; row < mli_Image_num_rows(img); row++) {
                for (col = 0; col < mli_Image_num_cols(img); col++) {
                        struct mli_Color color =
                                mli_Image_get_by_col_row(img, col, row);
                        struct mli_Color out =
                                mli_Color_truncate(color, 0., 255.);
                        uint8_t r = (uint8_t)out.r;
                        uint8_t g = (uint8_t)out.g;
                        uint8_t b = (uint8_t)out.b;
                        chk_IO_write(&r, sizeof(uint8_t), 1u, f);
                        chk_IO_write(&g, sizeof(uint8_t), 1u, f);
                        chk_IO_write(&b, sizeof(uint8_t), 1u, f);
                }
        }
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}
