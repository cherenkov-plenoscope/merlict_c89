/* Copyright 2018-2023 Sebastian Achim Mueller */
#ifndef mliIo_H_
#define mliIo_H_
#include <stdint.h>
#include <stdarg.h>
#include <stdio.h>
#include "../../mtl/src/string.h"

struct mliIo {
        /* memory */
        unsigned char *cstr;

        /* Capacity of the allocated memory */
        uint64_t capacity;

        /* Size of the payload in the allocated memory */
        uint64_t size;

        /* Position of the cursor */
        uint64_t pos;
};

struct mliIo mliIo_init(void);
void mliIo_free(struct mliIo *byt);
int mliIo_reset(struct mliIo *byt);
int mliIo_shrink_to_fit(struct mliIo *byt);

int mliIo_malloc(struct mliIo *byt);
int mliIo_malloc_capacity(struct mliIo *byt, const uint64_t capacity);
int mliIo_realloc_capacity(struct mliIo *byt, const uint64_t new_capacity);

/* copy */
/* ---- */
int mliIo_copy(struct mliIo *dst, const struct mliIo *src);
int mliIo_copy_start_num(
        struct mliIo *dst,
        const struct mliIo *src,
        const uint64_t start,
        const uint64_t length);

/* writing */
/* ------- */
int mliIo_write_cstr_format(struct mliIo *byt, const char *format, ...);
int mliIo_write_from_path(struct mliIo *byt, const char *path);
int mliIo_write_from_file(struct mliIo *byt, FILE *f, const uint64_t size);
int mliIo_write_unsigned_char(struct mliIo *byt, const unsigned char c);
int mliIo_write_char(struct mliIo *byt, const char c);
int64_t mliIo_write(
        struct mliIo *byt,
        const void *ptr,
        const uint64_t size,
        const uint64_t count);

/* reading */
/* ------- */
int mliIo_read_to_path(struct mliIo *byt, const char *path);
int mliIo_read_to_file(struct mliIo *byt, FILE *f, const uint64_t size);
int mliIo_read_char(struct mliIo *byt);
int64_t mliIo_read(
        struct mliIo *byt,
        const void *ptr,
        const uint64_t size,
        const uint64_t count);

/* file like */
/* --------- */
uint64_t mliIo_ftell(struct mliIo *byt);
void mliIo_rewind(struct mliIo *byt);
void mliIo_seek(struct mliIo *byt, const uint64_t pos);

int mliIo_readline(
        struct mliIo *stream,
        struct mliIo *line,
        const char delimiter);

int mli_readline(
        struct mliIo *stream,
        struct mtl_String *line,
        const char delimiter);

int mtl_String_convert_line_break_CRLF_CR_to_LF(
        struct mtl_String *dst,
        const struct mtl_String *src);

int mli_line_viewer_write(
        struct mliIo *f,
        const struct mtl_String *text,
        const uint64_t line_number,
        const uint64_t line_radius);

#endif
