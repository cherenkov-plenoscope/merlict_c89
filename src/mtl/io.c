/* Copyright 2018-2023 Sebastian Achim Mueller */
#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include "../chk/chk.h"
#include "vector.h"
#include "cstr.h"

struct mtl_IO mtl_IO_init(void)
{
        struct mtl_IO byt;
        byt.cstr = NULL;
        byt.capacity = 0u;
        byt.size = 0u;
        byt.pos = 0u;
        return byt;
}

void mtl_IO_free(struct mtl_IO *byt)
{
        free(byt->cstr);
        (*byt) = mtl_IO_init();
}

int mtl_IO_malloc_capacity(struct mtl_IO *byt, const uint64_t capacity)
{
        mtl_IO_free(byt);
        byt->capacity = MTL_MAX2(2u, capacity);
        byt->size = 0u;
        chk_malloc(byt->cstr, unsigned char, byt->capacity + 1u);
        memset(byt->cstr, '\0', byt->capacity + 1u);
        return 1;
chk_error:
        return 0;
}

int mtl_IO_malloc(struct mtl_IO *byt)
{
        chk(mtl_IO_malloc_capacity(byt, 0u));
        return 1;
chk_error:
        return 0;
}

int mtl_IO_realloc_capacity(struct mtl_IO *byt, const uint64_t new_capacity)
{
        uint64_t numcpy;
        struct mtl_IO tmp = mtl_IO_init();

        chk_msg(mtl_IO_malloc_capacity(&tmp, byt->capacity),
                "Failed to allocate temporary swap.");
        memcpy(tmp.cstr, byt->cstr, byt->capacity);
        tmp.pos = byt->pos;
        tmp.size = byt->size;

        chk_msg(mtl_IO_malloc_capacity(byt, new_capacity),
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

        mtl_IO_free(&tmp);
        return 1;
chk_error:
        return 0;
}

int mtl_IO_reset(struct mtl_IO *byt)
{
        if (byt->cstr == NULL) {
                chk(mtl_IO_malloc(byt));
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

int mtl_IO_shrink_to_fit(struct mtl_IO *byt)
{
        chk_msg(mtl_IO_realloc_capacity(byt, byt->size),
                "Failed to reallocate to size.");
        return 1;
chk_error:
        return 0;
}

int mtl_IO_write_unsigned_char(struct mtl_IO *byt, const unsigned char c)
{
        /* 'puts' a single byte (char) to the BytesIo-buffer */
        const uint64_t new_size = byt->size + 1u;

        if (byt->cstr == NULL) {
                chk(mtl_IO_malloc(byt));
        }

        if (new_size >= byt->capacity) {
                const uint64_t min_new_capacity =
                        MTL_MAX2(new_size, 2 * byt->capacity);
                chk_msg(mtl_IO_realloc_capacity(byt, min_new_capacity),
                        "Failed to reallocate.");
        }
        byt->cstr[byt->size] = c;
        byt->size = new_size;
        byt->pos += 1;

        return 1;
chk_error:
        return 0;
}

int mtl_IO_write_char(struct mtl_IO *byt, const char c)
{
        return mtl_IO_write_unsigned_char(byt, (unsigned char)c);
}

int mtl_IO_write_cstr_format(struct mtl_IO *str, const char *format, ...)
{
        uint64_t i;
        uint64_t tmp_length;
        struct mtl_IO tmp = mtl_IO_init();
        va_list args;

        chk(mtl_IO_malloc_capacity(&tmp, 32 * strlen(format)));
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
                chk(mtl_IO_write_unsigned_char(str, tmp.cstr[i]))
        }

        va_end(args);
        mtl_IO_free(&tmp);
        return 1;
chk_error:
        va_end(args);
        mtl_IO_free(&tmp);
        return 0;
}

/* copy */
/* ---- */

int mtl_IO_copy(struct mtl_IO *dst, const struct mtl_IO *src)
{
        return mtl_IO_copy_start_num(dst, src, 0, src->size);
}

int mtl_IO_copy_start_num(
        struct mtl_IO *dst,
        const struct mtl_IO *src,
        const uint64_t start,
        const uint64_t num)
{
        uint64_t i;
        chk_msg(src->cstr != NULL, "Expected src to be allocated");
        chk_msg(start + num <= src->size, "Expected start + num < src->size.");
        chk(mtl_IO_malloc_capacity(dst, num));
        for (i = 0; i < num; i++) {
                chk(mtl_IO_write_unsigned_char(dst, src->cstr[i + start]));
        }
        return 1;
chk_error:
        mtl_IO_free(dst);
        return 0;
}

int mtl_IO_read_char(struct mtl_IO *byt)
{
        int c;
        if (byt->pos >= byt->size) {
                return EOF;
        }
        c = byt->cstr[byt->pos];
        byt->pos += 1;
        return c;
}

int64_t mtl_IO_write(
        struct mtl_IO *byt,
        const void *ptr,
        const uint64_t size,
        const uint64_t count)
{
        const uint64_t block_size = size * count;
        unsigned char *block = (unsigned char *)ptr;

        uint64_t i;
        for (i = 0u; i < block_size; i++) {
                chk_msg(mtl_IO_write_unsigned_char(byt, block[i]),
                        "Failed to put byte");
        }
        return (i + 1u) / size;
chk_error:
        return -1;
}

int64_t mtl_IO_read(
        struct mtl_IO *byt,
        const void *ptr,
        const uint64_t size,
        const uint64_t count)
{
        const uint64_t block_size = size * count;
        unsigned char *block = (unsigned char *)ptr;

        uint64_t i;
        for (i = 0u; i < block_size; i++) {
                int c = mtl_IO_read_char(byt);

                if (c == EOF) {
                        return EOF;
                } else {
                        block[i] = (unsigned char)c;
                }
        }
        return (i + 1u) / size;
}

uint64_t mtl_IO_ftell(struct mtl_IO *byt) { return byt->pos; }

void mtl_IO_seek(struct mtl_IO *byt, const uint64_t pos) { byt->pos = pos; }

void mtl_IO_rewind(struct mtl_IO *byt) { byt->pos = 0u; }

int mtl_String_convert_line_break_CRLF_CR_to_LF(
        struct mtl_String *dst,
        const struct mtl_String *src)
{
        uint64_t i = 0;
        struct mtl_IO sdst = mtl_IO_init();
        chk(mtl_IO_malloc(&sdst));

        while (i < src->size) {
                if (mtl_cstr_is_CRLF((char *)&src->array[i])) {
                        chk(mtl_IO_write_char(&sdst, '\n'));
                        i += 2;
                } else if (mtl_cstr_is_CR((char *)&src->array[i])) {
                        chk(mtl_IO_write_char(&sdst, '\n'));
                        i += 1;
                } else {
                        chk(mtl_IO_write_char(&sdst, src->array[i]));
                        i += 1;
                }
        }

        chk(mtl_String_malloc(dst, sdst.size));
        strncpy(dst->array, (char *)sdst.cstr, sdst.size);

        mtl_IO_free(&sdst);
        return 1;
chk_error:
        mtl_IO_free(&sdst);
        mtl_String_free(dst);
        return 0;
}

int mtl_IO_write_from_path(struct mtl_IO *byt, const char *path)
{
        int c = EOF;
        FILE *f = fopen(path, "rt");
        chk_msg(f, "Failed to open file.");
        chk_msg(mtl_IO_malloc(byt), "Can not malloc string.");
        c = getc(f);
        while (c != EOF) {
                chk_msg(mtl_IO_write_char(byt, c), "Failed to push back char.");
                c = getc(f);
        }
        fclose(f);
        return 1;
chk_error:
        if (f != NULL)
                fclose(f);
        mtl_IO_free(byt);
        return 0;
}

int mtl_IO_read_to_path(struct mtl_IO *byt, const char *path)
{
        int c = 1;
        FILE *f = fopen(path, "w");
        chk_msg(f != NULL, "Failed to open path.");
        while (c != EOF) {
                c = mtl_IO_read_char(byt);
                chk(fputc(c, f));
        }
        fclose(f);
        return 1;
chk_error:
        fclose(f);
        return 0;
}

int mtl_IO_str_read_line(
        struct mtl_IO *stream,
        struct mtl_IO *line,
        const char delimiter)
{
        chk(mtl_IO_reset(line));

        while (stream->pos < stream->size) {
                const int c = mtl_IO_read_char(stream);
                if (c == '\0') {
                        break;
                } else if (c == delimiter) {
                        break;
                } else {
                        chk(mtl_IO_write_char(line, c));
                }
        }

        return 1;
chk_error:
        mtl_IO_free(line);
        return 0;
}

int mtl_IO_readline(
        struct mtl_IO *stream,
        struct mtl_String *line,
        const char delimiter)
{
        struct mtl_IO buf = mtl_IO_init();
        chk(mtl_IO_malloc(&buf));

        while (stream->pos < stream->size) {
                const int c = mtl_IO_read_char(stream);
                if (c == '\0') {
                        break;
                } else if (c == delimiter) {
                        break;
                } else {
                        chk(mtl_IO_write_char(&buf, c));
                }
        }

        mtl_IO_rewind(&buf);
        chk(mtl_String_malloc(line, buf.size));
        strcpy(line->array, (char *)buf.cstr);

        mtl_IO_free(&buf);
        return 1;
chk_error:
        mtl_IO_free(&buf);
        return 0;
}

int mli_line_viewer_write_line_match(
        struct mtl_IO *f,
        const int64_t line_number,
        const int64_t line_number_of_interest)
{
        chk(mtl_IO_write_cstr_format(f, "% 6d", (int32_t)line_number));
        if (line_number == line_number_of_interest) {
                chk(mtl_IO_write_cstr_format(f, "->|  "));
        } else {
                chk(mtl_IO_write_cstr_format(f, "  |  "));
        }
        return 1;
chk_error:
        return 0;
}

int mli_line_viewer_write(
        struct mtl_IO *f,
        const struct mtl_String *text,
        const uint64_t line_number,
        const uint64_t line_radius)
{
        int64_t _line_number = (int64_t)line_number;
        int64_t _line_radius = (int64_t)line_radius;
        int64_t line_start = MTL_MAX2(_line_number - _line_radius, 1);
        int64_t line_stop = line_number + line_radius;
        int64_t line = 1;
        uint64_t i = 0;

        chk_msg(line_radius > 1, "Expected line_radius > 1.");

        chk(mtl_IO_write_cstr_format(f, "  line     text\n"));
        chk(mtl_IO_write_cstr_format(f, "        |\n"));

        while (i < text->size && text->array[i]) {
                int prefix = (line + 1 >= line_start) && (line < line_stop);
                int valid = (line >= line_start) && (line <= line_stop);
                if (text->array[i] == '\n') {
                        line++;
                }
                if (prefix && i == 0) {
                        chk(mli_line_viewer_write_line_match(
                                f, line, _line_number));
                }
                if (valid) {
                        chk(mtl_IO_write_char(f, text->array[i]));
                }
                if (prefix && text->array[i] == '\n') {
                        chk(mli_line_viewer_write_line_match(
                                f, line, _line_number));
                }
                i++;
        }
        chk(mtl_IO_write_char(f, '\n'));

        return 1;
chk_error:
        return 0;
}

int mtl_IO_write_from_file(struct mtl_IO *byt, FILE *f, const uint64_t size)
{
        uint64_t i;
        chk_msg(f, "Expected file to be open.");
        chk_msg(byt->cstr != NULL, "Expected buffer to be allocated.");
        for (i = 0; i < size; i++) {
                char c = getc(f);
                chk(mtl_IO_write_unsigned_char(byt, c))
        }

        return 1;
chk_error:
        return 0;
}

int mtl_IO_read_to_file(struct mtl_IO *byt, FILE *f, const uint64_t size)
{
        uint64_t i;
        chk_msg(f, "Expected file to be open.");
        chk_msg(byt->cstr != NULL, "Expected buffer to be allocated.");
        for (i = 0; i < size; i++) {
                int rc = mtl_IO_read_char(byt);
                unsigned char c;
                chk(rc != EOF);
                c = (unsigned char)(rc);
                chk_fwrite(&c, sizeof(unsigned char), 1, f);
        }

        return 1;
chk_error:
        return 0;
}
