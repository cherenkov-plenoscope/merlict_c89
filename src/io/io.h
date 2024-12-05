/* Copyright 2018-2023 Sebastian Achim Mueller */
#ifndef MLI_IO_H_
#define MLI_IO_H_

#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include "../string/string.h"

struct mli_IO {
        /* memory */
        unsigned char *cstr;

        /* Capacity of the allocated memory */
        uint64_t capacity;

        /* Size of the payload in the allocated memory */
        uint64_t size;

        /* Position of the cursor */
        uint64_t pos;
};

struct mli_IO mli_IO_init(void);
void mli_IO_free(struct mli_IO *byt);
int mli_IO_reset(struct mli_IO *byt);
int mli_IO_shrink_to_fit(struct mli_IO *byt);

int mli_IO_malloc(struct mli_IO *byt);
int mli_IO_malloc_capacity(struct mli_IO *byt, const uint64_t capacity);
int mli_IO_realloc_capacity(struct mli_IO *byt, const uint64_t new_capacity);

/* copy */
/* ---- */
int mli_IO_copy(struct mli_IO *dst, const struct mli_IO *src);
int mli_IO_copy_start_num(
        struct mli_IO *dst,
        const struct mli_IO *src,
        const uint64_t start,
        const uint64_t length);

/* writing */
/* ------- */
int mli_IO_write_cstr_format(struct mli_IO *byt, const char *format, ...);
int mli_IO_write_from_path(struct mli_IO *byt, const char *path);
int mli_IO_write_from_file(struct mli_IO *byt, FILE *f, const uint64_t size);
int mli_IO_write_unsigned_char(struct mli_IO *byt, const unsigned char c);
int mli_IO_write_char(struct mli_IO *byt, const char c);
int64_t mli_IO_write(
        struct mli_IO *byt,
        const void *ptr,
        const uint64_t size,
        const uint64_t count);

/* reading */
/* ------- */
int mli_IO_read_to_path(struct mli_IO *byt, const char *path);
int mli_IO_read_to_file(struct mli_IO *byt, FILE *f, const uint64_t size);
int mli_IO_read_char(struct mli_IO *byt);
int64_t mli_IO_read(
        struct mli_IO *byt,
        const void *ptr,
        const uint64_t size,
        const uint64_t count);

/* file like */
/* --------- */
uint64_t mli_IO_ftell(struct mli_IO *byt);
void mli_IO_rewind(struct mli_IO *byt);
void mli_IO_seek(struct mli_IO *byt, const uint64_t pos);

int mli_IO_readline(
        struct mli_IO *stream,
        struct mli_String *line,
        const char delimiter);

int mli_String_convert_line_break_CRLF_CR_to_LF(
        struct mli_String *dst,
        const struct mli_String *src);

int mli_line_viewer_write(
        struct mli_IO *f,
        const struct mli_String *text,
        const uint64_t line_number,
        const uint64_t line_radius);

#endif
