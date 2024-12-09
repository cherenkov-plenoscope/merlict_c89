/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "io_text.h"
#include <stdio.h>
#include <stdlib.h>
#include "../chk/chk.h"
#include "../math/math.h"
#include "../cstr/cstr.h"

int mli_IO_text_getc(struct mli_IO *byt)
{
        unsigned char c;
        int rc = mli_IO__read_unsigned_char(byt, &c);
        if (rc == EOF) {
                return EOF;
        }
        return (int)c;
}

int mli_IO_text_read_line(
        struct mli_IO *stream,
        struct mli_String *line,
        const char delimiter)
{
        struct mli_IO buf = mli_IO_init();
        chk(mli_IO_reset(&buf));

        while (stream->pos < stream->size) {
                const int c = mli_IO_text_getc(stream);
                if (c == '\0') {
                        break;
                } else if (c == delimiter) {
                        break;
                } else {
                        chk(mli_IO_write_char(&buf, c));
                }
        }

        mli_IO_rewind(&buf);
        chk(mli_String_malloc(line, buf.size));
        strcpy(line->array, (char *)buf.cstr);

        mli_IO_free(&buf);
        return 1;
chk_error:
        mli_IO_free(&buf);
        return 0;
}

int mli_IO_text_write_multi_line_debug_view_line_match(
        struct mli_IO *f,
        const int64_t line_number,
        const int64_t line_number_of_interest)
{
        chk(mli_IO_write_cstr_format(f, "% 6d", (int32_t)line_number));
        if (line_number == line_number_of_interest) {
                chk(mli_IO_write_cstr_format(f, "->|  "));
        } else {
                chk(mli_IO_write_cstr_format(f, "  |  "));
        }
        return 1;
chk_error:
        return 0;
}

int mli_IO_text_write_multi_line_debug_view(
        struct mli_IO *f,
        const struct mli_String *text,
        const uint64_t line_number,
        const uint64_t line_radius)
{
        int64_t _line_number = (int64_t)line_number;
        int64_t _line_radius = (int64_t)line_radius;
        int64_t line_start = MLI_MATH_MAX2(_line_number - _line_radius, 1);
        int64_t line_stop = line_number + line_radius;
        int64_t line = 1;
        uint64_t i = 0;

        chk_msg(line_radius > 1, "Expected line_radius > 1.");

        chk(mli_IO_write_cstr_format(f, "  line     text\n"));
        chk(mli_IO_write_cstr_format(f, "        |\n"));

        while (i < text->size && text->array[i]) {
                int prefix = (line + 1 >= line_start) && (line < line_stop);
                int valid = (line >= line_start) && (line <= line_stop);
                if (text->array[i] == '\n') {
                        line++;
                }
                if (prefix && i == 0) {
                        chk(mli_IO_text_write_multi_line_debug_view_line_match(
                                f, line, _line_number));
                }
                if (valid) {
                        chk(mli_IO_write_char(f, text->array[i]));
                }
                if (prefix && text->array[i] == '\n') {
                        chk(mli_IO_text_write_multi_line_debug_view_line_match(
                                f, line, _line_number));
                }
                i++;
        }
        chk(mli_IO_write_char(f, '\n'));

        return 1;
chk_error:
        return 0;
}
