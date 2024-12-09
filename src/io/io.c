/* Copyright 2018-2023 Sebastian Achim Mueller */
#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include "../chk/chk.h"
#include "../math/math.h"
#include "../cstr/cstr.h"

struct mli_IO mli_IO_init(void)
{
        struct mli_IO byt;
        byt.cstr = NULL;
        byt.capacity = 0u;
        byt.size = 0u;
        byt.pos = 0u;
        return byt;
}

void mli_IO_free(struct mli_IO *byt)
{
        free(byt->cstr);
        (*byt) = mli_IO_init();
}

int mli_IO_malloc_capacity(struct mli_IO *byt, const uint64_t capacity)
{
        mli_IO_free(byt);
        byt->capacity = MLI_MATH_MAX2(2u, capacity);
        byt->size = 0u;
        chk_malloc(byt->cstr, unsigned char, byt->capacity + 1u);
        memset(byt->cstr, '\0', byt->capacity + 1u);
        return 1;
chk_error:
        return 0;
}

int mli_IO_malloc(struct mli_IO *byt)
{
        chk(mli_IO_malloc_capacity(byt, 0u));
        return 1;
chk_error:
        return 0;
}

int mli_IO_realloc_capacity(struct mli_IO *byt, const uint64_t new_capacity)
{
        uint64_t numcpy;
        struct mli_IO tmp = mli_IO_init();

        chk_msg(mli_IO_malloc_capacity(&tmp, byt->capacity),
                "Failed to allocate temporary swap.");
        memcpy(tmp.cstr, byt->cstr, byt->capacity);
        tmp.pos = byt->pos;
        tmp.size = byt->size;

        chk_msg(mli_IO_malloc_capacity(byt, new_capacity),
                "Faild to allocate new capacity.");

        if (new_capacity >= tmp.capacity) {
                /* growing or same */
                numcpy = tmp.capacity;
        } else {
                /* shrinking */
                numcpy = new_capacity;
                chk_msg(tmp.pos <= new_capacity,
                        "Expected cursor 'pos' to be within new capacity.");
                chk_msg(tmp.size <= new_capacity,
                        "Expected 'size' to be within new capacity.");
        }

        memcpy(byt->cstr, tmp.cstr, numcpy);
        byt->pos = tmp.pos;
        byt->size = tmp.size;

        mli_IO_free(&tmp);
        return 1;
chk_error:
        return 0;
}

int mli_IO_reset(struct mli_IO *byt)
{
        if (byt->cstr == NULL) {
                chk(mli_IO_malloc(byt));
        } else {
                chk_msg(byt->capacity >= 2u, "Expected minimal capacity of 2.");
                byt->pos = 0u;
                byt->size = 0u;
                memset(byt->cstr, '\0', byt->capacity + 1u);
        }
        return 1;
chk_error:
        return 0;
}

int mli_IO_shrink_to_fit(struct mli_IO *byt)
{
        chk_msg(mli_IO_realloc_capacity(byt, byt->size),
                "Failed to reallocate to size.");
        return 1;
chk_error:
        return 0;
}

int mli_IO_write_unsigned_char(struct mli_IO *byt, const unsigned char c)
{
        /* 'puts' a single byte (char) to the BytesIo-buffer */
        const uint64_t new_size = byt->size + 1u;

        if (byt->cstr == NULL) {
                chk(mli_IO_malloc(byt));
        }

        if (new_size >= byt->capacity) {
                const uint64_t min_new_capacity =
                        MLI_MATH_MAX2(new_size, 2 * byt->capacity);
                chk_msg(mli_IO_realloc_capacity(byt, min_new_capacity),
                        "Failed to reallocate.");
        }
        byt->cstr[byt->size] = c;
        byt->size = new_size;
        byt->pos += 1;

        return 1;
chk_error:
        return 0;
}

int mli_IO_write_char(struct mli_IO *byt, const char c)
{
        return mli_IO_write_unsigned_char(byt, (unsigned char)c);
}

int mli_IO_write_cstr_format(struct mli_IO *str, const char *format, ...)
{
        uint64_t i;
        uint64_t tmp_length;
        struct mli_IO tmp = mli_IO_init();
        va_list args;

        chk(mli_IO_malloc_capacity(&tmp, 32 * strlen(format)));
        va_start(args, format);
        vsprintf((char *)tmp.cstr, format, args);

        tmp_length = 0;
        for (i = 0; i < tmp.capacity; i++) {
                if (tmp.cstr[i] == (char)'\0') {
                        break;
                } else {
                        tmp_length += 1;
                }
        }
        chk_msg(tmp_length < tmp.capacity,
                "Expected tmp length < tmp.capacity. Probably vsprintf caused "
                "a buffer overflow.");

        for (i = 0; i < tmp_length; i++) {
                chk(mli_IO_write_unsigned_char(str, tmp.cstr[i]))
        }

        va_end(args);
        mli_IO_free(&tmp);
        return 1;
chk_error:
        va_end(args);
        mli_IO_free(&tmp);
        return 0;
}

/* copy */
/* ---- */

int mli_IO_copy(struct mli_IO *dst, const struct mli_IO *src)
{
        return mli_IO_copy_start_num(dst, src, 0, src->size);
}

int mli_IO_copy_start_num(
        struct mli_IO *dst,
        const struct mli_IO *src,
        const uint64_t start,
        const uint64_t num)
{
        uint64_t i;
        chk_msg(src->cstr != NULL, "Expected src to be allocated");
        chk_msg(start + num <= src->size, "Expected start + num < src->size.");
        chk(mli_IO_malloc_capacity(dst, num));
        for (i = 0; i < num; i++) {
                chk(mli_IO_write_unsigned_char(dst, src->cstr[i + start]));
        }
        return 1;
chk_error:
        mli_IO_free(dst);
        return 0;
}

int mli_IO_read_char(struct mli_IO *byt)
{
        int c;
        if (byt->pos >= byt->size) {
                return EOF;
        }
        c = byt->cstr[byt->pos];
        byt->pos += 1;
        return c;
}

int64_t mli_IO_write(
        struct mli_IO *byt,
        const void *ptr,
        const uint64_t size,
        const uint64_t count)
{
        const uint64_t block_size = size * count;
        unsigned char *block = (unsigned char *)ptr;

        uint64_t i;
        for (i = 0u; i < block_size; i++) {
                chk_msg(mli_IO_write_unsigned_char(byt, block[i]),
                        "Failed to put byte");
        }
        return (i + 1u) / size;
chk_error:
        return -1;
}

int64_t mli_IO_read(
        struct mli_IO *byt,
        const void *ptr,
        const uint64_t size,
        const uint64_t count)
{
        const uint64_t block_size = size * count;
        unsigned char *block = (unsigned char *)ptr;

        uint64_t i;
        for (i = 0u; i < block_size; i++) {
                int c = mli_IO_read_char(byt);

                if (c == EOF) {
                        return EOF;
                } else {
                        block[i] = (unsigned char)c;
                }
        }
        return (i + 1u) / size;
}

uint64_t mli_IO_ftell(struct mli_IO *byt) { return byt->pos; }

void mli_IO_seek(struct mli_IO *byt, const uint64_t pos) { byt->pos = pos; }

void mli_IO_rewind(struct mli_IO *byt) { byt->pos = 0u; }

int mli_IO_write_from_path(struct mli_IO *byt, const char *path)
{
        int c = EOF;
        FILE *f = fopen(path, "rt");
        chk_msg(f, "Failed to open file.");
        chk_msg(mli_IO_malloc(byt), "Can not malloc string.");
        c = getc(f);
        while (c != EOF) {
                chk_msg(mli_IO_write_char(byt, c), "Failed to push back char.");
                c = getc(f);
        }
        fclose(f);
        return 1;
chk_error:
        if (f != NULL)
                fclose(f);
        mli_IO_free(byt);
        return 0;
}

int mli_IO_read_to_path(struct mli_IO *byt, const char *path)
{
        int c = 1;
        FILE *f = fopen(path, "w");
        chk_msg(f != NULL, "Failed to open path.");
        while (c != EOF) {
                c = mli_IO_read_char(byt);
                chk(fputc(c, f));
        }
        fclose(f);
        return 1;
chk_error:
        fclose(f);
        return 0;
}

int mli_IO_str_read_line(
        struct mli_IO *stream,
        struct mli_IO *line,
        const char delimiter)
{
        chk(mli_IO_reset(line));

        while (stream->pos < stream->size) {
                const int c = mli_IO_read_char(stream);
                if (c == '\0') {
                        break;
                } else if (c == delimiter) {
                        break;
                } else {
                        chk(mli_IO_write_char(line, c));
                }
        }

        return 1;
chk_error:
        mli_IO_free(line);
        return 0;
}

int mli_IO_write_from_file(struct mli_IO *byt, FILE *f, const uint64_t size)
{
        uint64_t i;
        chk_msg(f, "Expected file to be open.");
        chk_msg(byt->cstr != NULL, "Expected buffer to be allocated.");
        for (i = 0; i < size; i++) {
                char c = getc(f);
                chk(mli_IO_write_unsigned_char(byt, c))
        }

        return 1;
chk_error:
        return 0;
}

int mli_IO_read_to_file(struct mli_IO *byt, FILE *f, const uint64_t size)
{
        uint64_t i;
        chk_msg(f, "Expected file to be open.");
        chk_msg(byt->cstr != NULL, "Expected buffer to be allocated.");
        for (i = 0; i < size; i++) {
                int rc = mli_IO_read_char(byt);
                unsigned char c;
                chk(rc != EOF);
                c = (unsigned char)(rc);
                chk_fwrite(&c, sizeof(unsigned char), 1, f);
        }

        return 1;
chk_error:
        return 0;
}
