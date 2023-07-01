/* Copyright 2018-2023 Sebastian Achim Mueller */
#ifndef mliIo_H_
#define mliIo_H_
#include <stdint.h>
#include <stdarg.h>
#include "mliStr.h"


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
int mliIo_malloc_capacity(struct mliIo *byt, const uint64_t capacity);
int mliIo_malloc(struct mliIo *byt);
int mliIo_putc(struct mliIo *byt, const unsigned char c);
int mliIo_putchar(struct mliIo *byt, const char c);
int mliIo_getc(struct mliIo *byt);
int64_t mliIo_write(
        struct mliIo *byt,
        const void *ptr,
        const uint64_t size,
        const uint64_t count);
int64_t mliIo_read(
        struct mliIo *byt,
        const void *ptr,
        const uint64_t size,
        const uint64_t count);

uint64_t mliIo_ftell(struct mliIo *byt);
void mliIo_rewind(struct mliIo *byt);
int64_t mliIo_printf(struct mliIo *byt, const char *format, ...);

int mliIo_convert_line_break_CRLF_CR_to_LF(
        struct mliIo *dst,
        const struct mliIo *src);

int mliIo_malloc_from_path(struct mliIo *byt, const char *path);
int64_t mliIo_writec(struct mliIo *byt, const char *s);

int64_t mliIo_setf(struct mliIo *str, const char *format, ...);

int mli_readline(struct mliIo *stream, struct mliStr *line, const char delimiter);

#endif
