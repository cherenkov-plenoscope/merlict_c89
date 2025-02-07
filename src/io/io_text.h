/* Copyright 2018-2023 Sebastian Achim Mueller */
#ifndef MLI_IO_TEXT_H_
#define MLI_IO_TEXT_H_

#include <stdint.h>
#include "../chk/chk.h"
#include "../string/string.h"
#include "io.h"
#include <stdarg.h>

int64_t mli_IO_text_getc(struct mli_IO *self);
chk_rc mli_IO_text_putc(struct mli_IO *self, const char c);

chk_rc mli_IO_text_write_cstr(struct mli_IO *self, const char *cstr);
chk_rc mli_IO_text_write_cstr_format(
        struct mli_IO *self,
        const char *format,
        ...);

chk_rc mli_IO_text_read_string(struct mli_IO *self, struct mli_String *str);
chk_rc mli_IO_text_read_line(
        struct mli_IO *self,
        struct mli_String *line,
        const char delimiter);

chk_rc mli_IO_text_write_String(
        struct mli_IO *self,
        const struct mli_String *str);

chk_rc mli_IO_text_write_multi_line_debug_view(
        struct mli_IO *self,
        const struct mli_String *text,
        const uint64_t line_number,
        const uint64_t line_radius);

#endif
