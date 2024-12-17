/* Copyright 2018-2023 Sebastian Achim Mueller */
#ifndef MLI_IO_TEXT_H_
#define MLI_IO_TEXT_H_

#include <stdint.h>
#include "../string/string.h"
#include "io_memory.h"
#include <stdarg.h>

int mli_IO_text_getc(struct mli_IO *self);
int mli_IO_text_putc(struct mli_IO *self, const char c);

int mli_IO_text_write_cstr(struct mli_IO *self, const char *str);
int mli_IO_text_write_cstr_format(struct mli_IO *self, const char *format, ...);

int mli_IO_text_read_line(
        struct mli_IO *self,
        struct mli_String *line,
        const char delimiter);

int mli_IO_text_write_String(struct mli_IO *self, const struct mli_String *str);

int mli_IO_text_write_multi_line_debug_view(
        struct mli_IO *self,
        const struct mli_String *text,
        const uint64_t line_number,
        const uint64_t line_radius);

#endif
