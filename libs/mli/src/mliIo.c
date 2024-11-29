/* Copyright 2018-2023 Sebastian Achim Mueller */
#include "mliIo.h"
#include <stdio.h>
#include <stdlib.h>
#include "../../chk/src/chk.h"
#include "mli_cstr.h"
#include "mli_math.h"

struct mliIo mliIo_init(void)
{
        struct mliIo byt;
        byt.cstr = NULL;
        byt.capacity = 0u;
        byt.size = 0u;
        byt.pos = 0u;
        return byt;
}

void mliIo_free(struct mliIo *byt)
{
        free(byt->cstr);
        (*byt) = mliIo_init();
}

int mliIo_malloc_capacity(struct mliIo *byt, const uint64_t capacity)
{
        mliIo_free(byt);
        byt->capacity = MLI_MAX2(2u, capacity);
        byt->size = 0u;
        chk_malloc(byt->cstr, unsigned char, byt->capacity + 1u);
        memset(byt->cstr, '\0', byt->capacity + 1u);
        return 1;
chk_error:
        return 0;
}

int mliIo_malloc(struct mliIo *byt)
{
        chk(mliIo_malloc_capacity(byt, 0u));
        return 1;
chk_error:
        return 0;
}

int mliIo_realloc_capacity(struct mliIo *byt, const uint64_t new_capacity)
{
        uint64_t numcpy;
        struct mliIo tmp = mliIo_init();

        chk_msg(mliIo_malloc_capacity(&tmp, byt->capacity),
                "Failed to allocate temporary swap.");
        memcpy(tmp.cstr, byt->cstr, byt->capacity);
        tmp.pos = byt->pos;
        tmp.size = byt->size;

        chk_msg(mliIo_malloc_capacity(byt, new_capacity),
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

        mliIo_free(&tmp);
        return 1;
chk_error:
        return 0;
}

int mliIo_reset(struct mliIo *byt)
{
        if (byt->cstr == NULL) {
                chk(mliIo_malloc(byt));
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

int mliIo_shrink_to_fit(struct mliIo *byt)
{
        chk_msg(mliIo_realloc_capacity(byt, byt->size),
                "Failed to reallocate to size.");
        return 1;
chk_error:
        return 0;
}

int mliIo_write_unsigned_char(struct mliIo *byt, const unsigned char c)
{
        /* 'puts' a single byte (char) to the BytesIo-buffer */
        const uint64_t new_size = byt->size + 1u;

        if (byt->cstr == NULL) {
                chk(mliIo_malloc(byt));
        }

        if (new_size >= byt->capacity) {
                const uint64_t min_new_capacity =
                        MLI_MAX2(new_size, 2 * byt->capacity);
                chk_msg(mliIo_realloc_capacity(byt, min_new_capacity),
                        "Failed to reallocate.");
        }
        byt->cstr[byt->size] = c;
        byt->size = new_size;
        byt->pos += 1;

        return 1;
chk_error:
        return 0;
}

int mliIo_write_char(struct mliIo *byt, const char c)
{
        return mliIo_write_unsigned_char(byt, (unsigned char)c);
}

int mliIo_write_cstr_format(struct mliIo *str, const char *format, ...)
{
        uint64_t i;
        uint64_t tmp_length;
        struct mliIo tmp = mliIo_init();
        va_list args;

        chk(mliIo_malloc_capacity(&tmp, 32 * strlen(format)));
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
                chk(mliIo_write_unsigned_char(str, tmp.cstr[i]))
        }

        va_end(args);
        mliIo_free(&tmp);
        return 1;
chk_error:
        va_end(args);
        mliIo_free(&tmp);
        return 0;
}

/* copy */
/* ---- */

int mliIo_copy(struct mliIo *dst, const struct mliIo *src)
{
        return mliIo_copy_start_num(dst, src, 0, src->size);
}

int mliIo_copy_start_num(
        struct mliIo *dst,
        const struct mliIo *src,
        const uint64_t start,
        const uint64_t num)
{
        uint64_t i;
        chk_msg(src->cstr != NULL, "Expected src to be allocated");
        chk_msg(start + num <= src->size, "Expected start + num < src->size.");
        chk(mliIo_malloc_capacity(dst, num));
        for (i = 0; i < num; i++) {
                chk(mliIo_write_unsigned_char(dst, src->cstr[i + start]));
        }
        return 1;
chk_error:
        mliIo_free(dst);
        return 0;
}

int mliIo_read_char(struct mliIo *byt)
{
        int c;
        if (byt->pos >= byt->size) {
                return EOF;
        }
        c = byt->cstr[byt->pos];
        byt->pos += 1;
        return c;
}

int64_t mliIo_write(
        struct mliIo *byt,
        const void *ptr,
        const uint64_t size,
        const uint64_t count)
{
        const uint64_t block_size = size * count;
        unsigned char *block = (unsigned char *)ptr;

        uint64_t i;
        for (i = 0u; i < block_size; i++) {
                chk_msg(mliIo_write_unsigned_char(byt, block[i]),
                        "Failed to put byte");
        }
        return (i + 1u) / size;
chk_error:
        return -1;
}

int64_t mliIo_read(
        struct mliIo *byt,
        const void *ptr,
        const uint64_t size,
        const uint64_t count)
{
        const uint64_t block_size = size * count;
        unsigned char *block = (unsigned char *)ptr;

        uint64_t i;
        for (i = 0u; i < block_size; i++) {
                int c = mliIo_read_char(byt);

                if (c == EOF) {
                        return EOF;
                } else {
                        block[i] = (unsigned char)c;
                }
        }
        return (i + 1u) / size;
}

uint64_t mliIo_ftell(struct mliIo *byt) { return byt->pos; }

void mliIo_seek(struct mliIo *byt, const uint64_t pos) { byt->pos = pos; }

void mliIo_rewind(struct mliIo *byt) { byt->pos = 0u; }

int mliStr_convert_line_break_CRLF_CR_to_LF(
        struct mliStr *dst,
        const struct mliStr *src)
{
        uint64_t i = 0;
        struct mliIo sdst = mliIo_init();
        chk(mliIo_malloc(&sdst));

        while (i < src->length) {
                if (mli_cstr_is_CRLF((char *)&src->cstr[i])) {
                        chk(mliIo_write_char(&sdst, '\n'));
                        i += 2;
                } else if (mli_cstr_is_CR((char *)&src->cstr[i])) {
                        chk(mliIo_write_char(&sdst, '\n'));
                        i += 1;
                } else {
                        chk(mliIo_write_char(&sdst, src->cstr[i]));
                        i += 1;
                }
        }

        chk(mliStr_malloc(dst, sdst.size));
        strncpy(dst->cstr, (char *)sdst.cstr, sdst.size);

        mliIo_free(&sdst);
        return 1;
chk_error:
        mliIo_free(&sdst);
        mliStr_free(dst);
        return 0;
}

int mliIo_write_from_path(struct mliIo *byt, const char *path)
{
        int c = EOF;
        FILE *f = fopen(path, "rt");
        chk_msg(f, "Failed to open file.");
        chk_msg(mliIo_malloc(byt), "Can not malloc string.");
        c = getc(f);
        while (c != EOF) {
                chk_msg(mliIo_write_char(byt, c), "Failed to push back char.");
                c = getc(f);
        }
        fclose(f);
        return 1;
chk_error:
        if (f != NULL)
                fclose(f);
        mliIo_free(byt);
        return 0;
}

int mliIo_read_to_path(struct mliIo *byt, const char *path)
{
        int c = 1;
        FILE *f = fopen(path, "w");
        chk_msg(f != NULL, "Failed to open path.");
        while (c != EOF) {
                c = mliIo_read_char(byt);
                chk(fputc(c, f));
        }
        fclose(f);
        return 1;
chk_error:
        fclose(f);
        return 0;
}

int mliIo_str_read_line(
        struct mliIo *stream,
        struct mliIo *line,
        const char delimiter)
{
        chk(mliIo_reset(line));

        while (stream->pos < stream->size) {
                const int c = mliIo_read_char(stream);
                if (c == '\0') {
                        break;
                } else if (c == delimiter) {
                        break;
                } else {
                        chk(mliIo_write_char(line, c));
                }
        }

        return 1;
chk_error:
        mliIo_free(line);
        return 0;
}

int mli_readline(
        struct mliIo *stream,
        struct mliStr *line,
        const char delimiter)
{
        struct mliIo buf = mliIo_init();
        chk(mliIo_malloc(&buf));

        while (stream->pos < stream->size) {
                const int c = mliIo_read_char(stream);
                if (c == '\0') {
                        break;
                } else if (c == delimiter) {
                        break;
                } else {
                        chk(mliIo_write_char(&buf, c));
                }
        }

        mliIo_rewind(&buf);
        chk(mliStr_malloc(line, buf.size));
        strcpy(line->cstr, (char *)buf.cstr);

        mliIo_free(&buf);
        return 1;
chk_error:
        mliIo_free(&buf);
        return 0;
}

int mli_path_strip_this_dir(const struct mliStr *src, struct mliStr *dst)
{
        uint64_t i = 0;
        struct mliStr cpysrc = mliStr_init();
        chk_msg(src->cstr, "Expected src-string to be allocated.");
        chk_msg(mliStr_malloc_copy(&cpysrc, src), "Can not copy input.");
        mliStr_free(dst);

        if (cpysrc.cstr == NULL) {
                return 1;
        }

        while (i + 1 < cpysrc.length) {
                if (cpysrc.cstr[i] == '.' && cpysrc.cstr[i + 1] == '/') {
                        i += 2;
                } else {
                        break;
                }
        }
        chk(mliStr_malloc(dst, cpysrc.length - i));
        strcpy(dst->cstr, &cpysrc.cstr[i]);
        mliStr_free(&cpysrc);
        return 1;
chk_error:
        mliStr_free(&cpysrc);
        mliStr_free(dst);
        return 0;
}

int mli_path_basename(const struct mliStr *src, struct mliStr *dst)
{
        int64_t pos_last_del = -1;
        mliStr_free(dst);
        chk_msg(src->cstr != NULL, "Expected src-path to be allocated");

        pos_last_del = mliStr_rfind(src, '/');

        if (pos_last_del < 0) {
                chk(mliStr_mallocf(dst, src->cstr));
        } else {
                chk(mliStr_mallocf(dst, &src->cstr[pos_last_del + 1]));
        }
        return 1;
chk_error:
        mliStr_free(dst);
        return 0;
}

int mli_path_splitext(
        const struct mliStr *src,
        struct mliStr *dst,
        struct mliStr *ext)
{
        int64_t p = -1;
        int64_t d = -1;
        struct mliStr tmp = mliStr_init();
        chk_msg(src->cstr != NULL, "Expected src-path to be allocated");
        chk(mliStr_malloc_copy(&tmp, src));

        mliStr_free(dst);
        mliStr_free(ext);

        p = mliStr_rfind(&tmp, '.');
        d = mliStr_rfind(&tmp, '/');

        if (p <= 0 || d > p ||
            ((d + 1 == p) && (p + 1 < (int64_t)tmp.length))) {
                chk(mliStr_mallocf(dst, tmp.cstr));
                chk(mliStr_mallocf(ext, ""));
        } else {
                chk(mliStr_malloc(dst, p));
                strncpy(dst->cstr, tmp.cstr, p);
                chk(mliStr_malloc(ext, tmp.length - p));
                strncpy(ext->cstr, &tmp.cstr[p + 1], tmp.length - p);
        }

        mliStr_free(&tmp);
        return 1;
chk_error:
        mliStr_free(&tmp);
        mliStr_free(dst);
        mliStr_free(ext);
        return 0;
}

int mli_line_viewer_write_line_match(
        struct mliIo *f,
        const int64_t line_number,
        const int64_t line_number_of_interest)
{
        chk(mliIo_write_cstr_format(f, "% 6d", (int32_t)line_number));
        if (line_number == line_number_of_interest) {
                chk(mliIo_write_cstr_format(f, "->|  "));
        } else {
                chk(mliIo_write_cstr_format(f, "  |  "));
        }
        return 1;
chk_error:
        return 0;
}

int mli_line_viewer_write(
        struct mliIo *f,
        const struct mliStr *text,
        const uint64_t line_number,
        const uint64_t line_radius)
{
        int64_t _line_number = (int64_t)line_number;
        int64_t _line_radius = (int64_t)line_radius;
        int64_t line_start = MLI_MAX2(_line_number - _line_radius, 1);
        int64_t line_stop = line_number + line_radius;
        int64_t line = 1;
        uint64_t i = 0;

        chk_msg(line_radius > 1, "Expected line_radius > 1.");

        chk(mliIo_write_cstr_format(f, "  line     text\n"));
        chk(mliIo_write_cstr_format(f, "        |\n"));

        while (i < text->length && text->cstr[i]) {
                int prefix = (line + 1 >= line_start) && (line < line_stop);
                int valid = (line >= line_start) && (line <= line_stop);
                if (text->cstr[i] == '\n') {
                        line++;
                }
                if (prefix && i == 0) {
                        chk(mli_line_viewer_write_line_match(
                                f, line, _line_number));
                }
                if (valid) {
                        chk(mliIo_write_char(f, text->cstr[i]));
                }
                if (prefix && text->cstr[i] == '\n') {
                        chk(mli_line_viewer_write_line_match(
                                f, line, _line_number));
                }
                i++;
        }
        chk(mliIo_write_char(f, '\n'));

        return 1;
chk_error:
        return 0;
}

int mliIo_write_from_file(struct mliIo *byt, FILE *f, const uint64_t size)
{
        uint64_t i;
        chk_msg(f, "Expected file to be open.");
        chk_msg(byt->cstr != NULL, "Expected buffer to be allocated.");
        for (i = 0; i < size; i++) {
                char c = getc(f);
                chk(mliIo_write_unsigned_char(byt, c))
        }

        return 1;
chk_error:
        return 0;
}

int mliIo_read_to_file(struct mliIo *byt, FILE *f, const uint64_t size)
{
        uint64_t i;
        chk_msg(f, "Expected file to be open.");
        chk_msg(byt->cstr != NULL, "Expected buffer to be allocated.");
        for (i = 0; i < size; i++) {
                int rc = mliIo_read_char(byt);
                unsigned char c;
                chk(rc != EOF);
                c = (unsigned char)(rc);
                chk_fwrite(&c, sizeof(unsigned char), 1, f);
        }

        return 1;
chk_error:
        return 0;
}
