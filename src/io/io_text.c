/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "io_text.h"
#include <stdio.h>
#include <stdlib.h>
#include "../chk/chk.h"
#include "../math/math.h"
#include "../cstr/cstr.h"

int mli_IO_text_getc(struct mli_IO *byt)
{
        char c = EOF;
        size_t rc = mli_IO_read(byt, (void *)(&c), sizeof(char), 1);
        if (rc == 0) {
                return EOF;
        } else {
                return (int)c;
        }
}

int mli_IO_text_putc(struct mli_IO *byt, const char c)
{
        chk_msg(mli_IO_write(byt, (void *)(&c), sizeof(char), 1),
                "Can not write char to IO.");
        return 1;
chk_error:
        return 0;
}

int mli_IO_text_write_cstr_format(struct mli_IO *self, const char *format, ...)
{
        struct mli_String tmp = mli_String_init();
        va_list args;
        va_start(args, format);
        chk(mli_String_from_vargs(&tmp, format, args));
        chk(mli_IO_text_write_String(self, &tmp));
        va_end(args);
        mli_String_free(&tmp);
        return 1;
chk_error:
        va_end(args);
        mli_String_free(&tmp);
        return 0;
}

int mli_IO_text_read_line(
        struct mli_IO *stream,
        struct mli_String *line,
        const char delimiter)
{
        chk(mli_String_malloc(line, 16));

        if (mli_IO_eof(stream)) {
                return EOF;
        }

        while (!mli_IO_eof(stream)) {
                const int c = mli_IO_text_getc(stream);
                if (c == delimiter) {
                        break;
                } else if (c == '\0') {
                        break;
                } else {
                        chk(mli_String_push_back(line, c));
                }
        }

        return 1;
chk_error:
        return 0;
}

int mli_IO_text_write_String(struct mli_IO *self, const struct mli_String *str)
{
        uint64_t i;
        for (i = 0; i < str->size; i++) {
                char c = str->array[i];
                if (c == '\0') {
                        break;
                }
                chk(mli_IO_text_putc(self, c));
        }
        return 1;
chk_error:
        return 0;
}

int mli_IO_text_write_multi_line_debug_view_line_match(
        struct mli_IO *f,
        const int64_t line_number,
        const int64_t line_number_of_interest)
{
        chk(mli_IO_text_write_cstr_format(f, "% 6d", (int32_t)line_number));
        if (line_number == line_number_of_interest) {
                chk(mli_IO_text_write_cstr_format(f, "->|  "));
        } else {
                chk(mli_IO_text_write_cstr_format(f, "  |  "));
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

        chk(mli_IO_text_write_cstr_format(f, "  line     text\n"));
        chk(mli_IO_text_write_cstr_format(f, "        |\n"));

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
                        chk(mli_IO_text_putc(f, text->array[i]));
                }
                if (prefix && text->array[i] == '\n') {
                        chk(mli_IO_text_write_multi_line_debug_view_line_match(
                                f, line, _line_number));
                }
                i++;
        }
        chk(mli_IO_text_putc(f, '\n'));

        return 1;
chk_error:
        return 0;
}
