/* Copyright 2018-2023 Sebastian Achim Mueller */
#ifndef mtl_IO_H_
#define mtl_IO_H_
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include "../string/string.h"

struct mtl_IO {
        /* memory */
        unsigned char *cstr;

        /* Capacity of the allocated memory */
        uint64_t capacity;

        /* Size of the payload in the allocated memory */
        uint64_t size;

        /* Position of the cursor */
        uint64_t pos;
};

struct mtl_IO mtl_IO_init(void);
void mtl_IO_free(struct mtl_IO *byt);
int mtl_IO_reset(struct mtl_IO *byt);
int mtl_IO_shrink_to_fit(struct mtl_IO *byt);

int mtl_IO_malloc(struct mtl_IO *byt);
int mtl_IO_malloc_capacity(struct mtl_IO *byt, const uint64_t capacity);
int mtl_IO_realloc_capacity(struct mtl_IO *byt, const uint64_t new_capacity);

/* copy */
/* ---- */
int mtl_IO_copy(struct mtl_IO *dst, const struct mtl_IO *src);
int mtl_IO_copy_start_num(
        struct mtl_IO *dst,
        const struct mtl_IO *src,
        const uint64_t start,
        const uint64_t length);

/* writing */
/* ------- */
int mtl_IO_write_cstr_format(struct mtl_IO *byt, const char *format, ...);
int mtl_IO_write_from_path(struct mtl_IO *byt, const char *path);
int mtl_IO_write_from_file(struct mtl_IO *byt, FILE *f, const uint64_t size);
int mtl_IO_write_unsigned_char(struct mtl_IO *byt, const unsigned char c);
int mtl_IO_write_char(struct mtl_IO *byt, const char c);
int64_t mtl_IO_write(
        struct mtl_IO *byt,
        const void *ptr,
        const uint64_t size,
        const uint64_t count);

/* reading */
/* ------- */
int mtl_IO_read_to_path(struct mtl_IO *byt, const char *path);
int mtl_IO_read_to_file(struct mtl_IO *byt, FILE *f, const uint64_t size);
int mtl_IO_read_char(struct mtl_IO *byt);
int64_t mtl_IO_read(
        struct mtl_IO *byt,
        const void *ptr,
        const uint64_t size,
        const uint64_t count);

/* file like */
/* --------- */
uint64_t mtl_IO_ftell(struct mtl_IO *byt);
void mtl_IO_rewind(struct mtl_IO *byt);
void mtl_IO_seek(struct mtl_IO *byt, const uint64_t pos);

int mtl_IO_readline(
        struct mtl_IO *stream,
        struct mli_String *line,
        const char delimiter);

int mli_String_convert_line_break_CRLF_CR_to_LF(
        struct mli_String *dst,
        const struct mli_String *src);

int mli_line_viewer_write(
        struct mtl_IO *f,
        const struct mli_String *text,
        const uint64_t line_number,
        const uint64_t line_radius);

#endif
