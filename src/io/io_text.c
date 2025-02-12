/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "io_text.h"
#include <stdio.h>
#include <stdlib.h>
#include "../chk/chk.h"
#include "../math/math.h"
#include "../cstr/cstr.h"

int64_t mli_IO_text_getc(struct mli_IO *self)
{
        char c = EOF;
        size_t rc = mli_IO_read((void *)(&c), sizeof(char), 1, self);
        if (rc == 0) {
                return EOF;
        } else {
                return (int64_t)c;
        }
}

chk_rc mli_IO_text_putc(struct mli_IO *self, const char c)
{
        chk_msg(mli_IO_write((void *)(&c), sizeof(char), 1, self),
                "Can not write char to IO.");
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_IO_text_write_cstr(struct mli_IO *self, const char *cstr)
{
        struct mli_String tmp = mli_String_init();
        chk_msg(mli_String_from_cstr(&tmp, cstr),
                "Can't malloc mli_String from cstr");
        chk(mli_IO_text_write_String(self, &tmp));
        mli_String_free(&tmp);
        return CHK_SUCCESS;
chk_error:
        mli_String_free(&tmp);
        return CHK_FAIL;
}

chk_rc mli_IO_text_write_cstr_format(
        struct mli_IO *self,
        const char *format,
        ...)
{
        struct mli_String tmp = mli_String_init();
        va_list args;
        va_start(args, format);
        chk(mli_String_from_vargs(&tmp, format, args));
        chk(mli_IO_text_write_String(self, &tmp));
        va_end(args);
        mli_String_free(&tmp);
        return CHK_SUCCESS;
chk_error:
        va_end(args);
        mli_String_free(&tmp);
        return CHK_FAIL;
}

chk_rc mli_IO_text_read_string(struct mli_IO *self, struct mli_String *str)
{
        chk(mli_IO_text_read_line(self, str, '\0'));
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_IO_text_read_line(
        struct mli_IO *self,
        struct mli_String *line,
        const char delimiter)
{
        chk(mli_String_malloc(line, 0));

        if (mli_IO_eof(self)) {
                return EOF;
        }

        while (!mli_IO_eof(self)) {
                const int c = mli_IO_text_getc(self);
                if (c == delimiter) {
                        break;
                } else if (c == '\0') {
                        break;
                } else if (c == EOF) {
                        break;
                } else {
                        chk(mli_String_push_back(line, c));
                }
        }

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_IO_text_write_String(
        struct mli_IO *self,
        const struct mli_String *str)
{
        uint64_t i;
        for (i = 0; i < str->size; i++) {
                char c = str->array[i];
                if (c == '\0') {
                        break;
                }
                chk(mli_IO_text_putc(self, c));
        }
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_IO_text_write_multi_line_debug_view_line_match(
        struct mli_IO *self,
        const int64_t line_number,
        const int64_t line_number_of_interest)
{
        chk(mli_IO_text_write_cstr_format(self, "% 6d", (int32_t)line_number));
        if (line_number == line_number_of_interest) {
                chk(mli_IO_text_write_cstr_format(self, "->|  "));
        } else {
                chk(mli_IO_text_write_cstr_format(self, "  |  "));
        }
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_IO_text_write_multi_line_debug_view(
        struct mli_IO *self,
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

        chk(mli_IO_text_write_cstr_format(self, "  line     text\n"));
        chk(mli_IO_text_write_cstr_format(self, "        |\n"));

        while (i < text->size && text->array[i]) {
                int prefix = (line + 1 >= line_start) && (line < line_stop);
                int valid = (line >= line_start) && (line <= line_stop);
                if (text->array[i] == '\n') {
                        line++;
                }
                if (prefix && i == 0) {
                        chk(mli_IO_text_write_multi_line_debug_view_line_match(
                                self, line, _line_number));
                }
                if (valid) {
                        chk(mli_IO_text_putc(self, text->array[i]));
                }
                if (prefix && text->array[i] == '\n') {
                        chk(mli_IO_text_write_multi_line_debug_view_line_match(
                                self, line, _line_number));
                }
                i++;
        }
        chk(mli_IO_text_putc(self, '\n'));

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}
