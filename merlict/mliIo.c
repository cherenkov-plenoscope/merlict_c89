/* Copyright 2018-2023 Sebastian Achim Mueller */
#include "mliIo.h"
#include <stdio.h>


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
        chk_malloc(byt->cstr, unsigned char, byt->capacity);
        memset(byt->cstr, '\0', byt->capacity);
        return 1;
error:
        return 0;
}

int mliIo_malloc(struct mliIo *byt)
{
        chk(mliIo_malloc_capacity(byt, 0u));
        return 1;
error:
        return 0;
}

int mliIo_putc(struct mliIo *byt, const unsigned char c)
{
        /* 'puts' a single byte (char) to the BytesIo-buffer */
        const uint64_t new_size = byt->size + 1u;

        if (byt->cstr == NULL) {
                chk(mliIo_malloc(byt));
        }

        if (new_size >= byt->capacity) {
                const uint64_t min_new_capacity =
                        MLI_MAX2(new_size, 2 * byt->capacity);

                byt->capacity = min_new_capacity;
                byt->cstr = (unsigned char *)realloc(
                        (void *)byt->cstr, byt->capacity * sizeof(char));
                memset(&byt->cstr[byt->size], '\0', byt->capacity - byt->size);
                chk_mem(byt->cstr);
        }
        byt->cstr[byt->size] = c;
        byt->size = new_size;
        byt->pos += 1;

        return 1;
error:
        return 0;
}

int mliIo_putchar(struct mliIo *byt, const char c)
{
        return mliIo_putc(byt, (unsigned char)c);
}

int mliIo_getc(struct mliIo *byt)
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
                chk_msg(mliIo_putc(byt, block[i]), "Failed to put byte");
        }
        return (i + 1u) / size;
error:
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
                int c = mliIo_getc(byt);

                if (c == EOF) {
                        return EOF;
                } else {
                        block[i] = (unsigned char)c;
                }
        }
        return (i + 1u) / size;
}

uint64_t mliIo_ftell(struct mliIo *byt) { return byt->pos; }

void mliIo_rewind(struct mliIo *byt) { byt->pos = 0u; }

int64_t mliIo_printf(struct mliIo *byt, const char *format, ...)
{
        uint64_t i;
        char tmp[256];
        va_list args;
        memset(tmp, '\0', sizeof(tmp));

        va_start(args, format);

        /* DANGER, THIS WILL 100% BUFFEROVERFLOW  */
        /* PROBLEM IS THERE IS NO vsnprintf (note the 'n') in c89 */
        /* SO SEFAULT-PARADE IT IS UNTIL I WRITE MY OWN snprintf */
        vsprintf(tmp, format, args);

        for (i = 0; i < 256; i++) {
                if (tmp[i] == '\0') {
                        break;
                }
                chk(mliIo_putchar(byt, tmp[i]));
        }
        va_end(args);
        return (int64_t)i;
error:
        va_end(args);
        return -1;
}

int mliIo_convert_line_break_CRLF_CR_to_LF(
        struct mliIo *dst,
        const struct mliIo *src)
{
        uint64_t i = 0;
        mliIo_free(dst);
        chk(mliIo_malloc_capacity(dst, src->capacity));

        while (i < src->capacity) {
                if (mli_cstr_is_CRLF((char *)&src->cstr[i])) {
                        chk(mliIo_putchar(dst, '\n'));
                        i += 2;
                } else if (mli_cstr_is_CR((char *)&src->cstr[i])) {
                        chk(mliIo_putchar(dst, '\n'));
                        i += 1;
                } else {
                        chk(mliIo_putchar(dst, src->cstr[i]));
                        i += 1;
                }
        }

        return 1;
error:
        mliIo_free(dst);
        return 0;
}

int mliIo_malloc_from_path(struct mliIo *byt, const char *path)
{
        int c = EOF;
        FILE *f = fopen(path, "rt");
        chk_msg(f, "Failed to open file.");
        chk_msg(mliIo_malloc(byt), "Can not malloc string.");
        c = getc(f);
        while (c != EOF) {
                chk_msg(mliIo_putchar(byt, c), "Failed to push back char.");
                c = getc(f);
        }
        fclose(f);
        return 1;
error:
        if (f != NULL)
                fclose(f);
        mliIo_free(byt);
        return 0;
}

int64_t mliIo_malloc_cstr(struct mliIo *byt, const char *s)
{
        const uint64_t slen = strlen(s);
        uint64_t i;
        for (i = 0; i < slen; i++) {
                chk_msg(mliIo_putchar(byt, s[i]), "Failed to push back char");
        }
        return i;
error:
        return 0;
}

int mli_readline(struct mliIo *stream, struct mliStr *line, const char delimiter)
{
        struct mliIo buf = mliIo_init();
        chk(mliIo_malloc(&buf));

        while (stream->pos < stream->size) {
                const int c = mliIo_getc(stream);
                if (c == '\0') {
                        break;
                } else if (c == delimiter) {
                        break;
                } else {
                        chk(mliIo_putchar(&buf, c));
                }
        }

        mliIo_rewind(&buf);
        chk(mliStr_malloc(line, buf.size));
        strcpy(line->cstr, (char *)buf.cstr);

        mliIo_free(&buf);
        return 1;
error:
        mliIo_free(&buf);
        return 0;
}

int mli_path_strip_this_dir(const struct mliStr *src, struct mliStr *dst)
{
        uint64_t i = 0;
        mliStr_free(dst);
        if (src->cstr == NULL) {
                return 1;
        }

        while (i + 1 < src->length) {
                if (src->cstr[i] == '.' && src->cstr[i + 1] == '/') {
                        i += 2;
                } else {
                        break;
                }
        }
        chk(mliStr_malloc(dst, src->length - i));
        strcpy(dst->cstr, &src->cstr[i]);
        return 1;
error:
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
error:
        mliStr_free(dst);
        return 0;
}

int mli_path_splitext(const struct mliStr *src, struct mliStr *dst, struct mliStr *ext)
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

        if (p <= 0 || d > p || ((d + 1 == p) && (p + 1 < (int64_t)tmp.length))) {
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
error:
        mliStr_free(&tmp);
        mliStr_free(dst);
        mliStr_free(ext);
        return 0;
}
