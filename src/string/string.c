/* Copyright Sebastian Achim Mueller */
#include "string.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "../chk/chk.h"
#include "../cstr/cstr.h"
#include "../io/io.h"

MLI_ARRAY_IMPLEMENTATION_ZERO_TERMINATION(mli_String, char)

int mli_String_from_cstr_fromat(struct mli_String *str, const char *format, ...)
{
        struct mli_String tmp = mli_String_init();
        va_list args;

        chk(mli_String_malloc(&tmp, 10 * strlen(format)));
        va_start(args, format);
        vsprintf(tmp.array, format, args);
        chk(mli_String_malloc(str, strlen(tmp.array)));
        strncpy(str->array, tmp.array, str->size);
        va_end(args);
        mli_String_free(&tmp);
        return 1;
chk_error:
        va_end(args);
        mli_String_free(&tmp);
        return 0;
}

int mli_String_from_cstr(struct mli_String *str, const char *s)
{
        chk(mli_String_malloc(str, strlen(s)));
        strncpy(str->array, s, str->size);
        return 1;
chk_error:
        return 0;
}

int mli_String_ends_with(
        const struct mli_String *str,
        const struct mli_String *suffix)
{
        if (!str->array || !suffix->array) {
                return 0;
        }
        if (suffix->size > str->size) {
                return 0;
        }
        return strncmp(str->array + str->size - suffix->size,
                       suffix->array,
                       suffix->size) == 0;
}

int mli_String_starts_with(
        const struct mli_String *str,
        const struct mli_String *prefix)
{
        if (!str->array || !prefix->array) {
                return 0;
        }
        if (prefix->size > str->size) {
                return 0;
        }
        return strncmp(str->array, prefix->array, prefix->size) == 0;
}

int mli_String_has_prefix_suffix(
        const struct mli_String *str,
        const struct mli_String *prefix,
        const struct mli_String *suffix)
{
        uint64_t has_pre = 1;
        uint64_t has_suf = 1;
        if (prefix->array != NULL) {
                has_pre = mli_String_starts_with(str, prefix);
        }

        if (suffix->array != NULL) {
                has_suf = mli_String_ends_with(str, suffix);
        }

        if (has_pre == 1 && has_suf == 1) {
                return 1;
        } else {
                return 0;
        }
}

int64_t mli_String_rfind(const struct mli_String *str, const char c)
{
        int64_t i;
        for (i = str->size - 1; i >= 0; i--) {
                if (str->array[i] == '\0') {
                        return -1;
                } else if (str->array[i] == c) {
                        return i;
                }
        }
        return -1;
}

int64_t mli_String_find(const struct mli_String *str, const char c)
{
        int64_t i;
        for (i = 0; i < (int64_t)str->size; i++) {
                if (str->array[i] == '\0') {
                        return -1;
                } else if (str->array[i] == c) {
                        return i;
                }
        }
        return -1;
}

int mli_String_match_templeate(
        const struct mli_String *s,
        const struct mli_String *t,
        const char digit_wildcard)
{
        uint64_t i;
        if (s->size != t->size) {
                return 0;
        }
        for (i = 0; i < s->size; i++) {
                if (t->array[i] == digit_wildcard) {
                        if (!isdigit(s->array[i])) {
                                return 0;
                        }
                } else {
                        if (s->array[i] != t->array[i]) {
                                return 0;
                        }
                }
        }
        return 1;
}

int mli_String_strip(const struct mli_String *src, struct mli_String *dst)
{
        int64_t start = 0;
        int64_t stop = 0;
        int64_t len = -1;
        struct mli_String cpysrc = mli_String_init();

        chk_msg(src->array, "Expected src-string to be allocated.");
        chk_msg(mli_String_copy(&cpysrc, src), "Can not copy input.");
        mli_String_free(dst);

        while (start < (int64_t)cpysrc.size && isspace(cpysrc.array[start])) {
                start += 1;
        }

        stop = cpysrc.size - 1;
        while (stop >= 0 && isspace(cpysrc.array[stop])) {
                stop -= 1;
        }

        len = stop - start;

        if (len < 0) {
                chk(mli_String_from_cstr_fromat(dst, ""));
        } else {
                chk(mli_String_malloc(dst, len + 1));
                strncpy(dst->array, &cpysrc.array[start], len + 1);
        }
        mli_String_free(&cpysrc);
        return 1;
chk_error:
        mli_String_free(&cpysrc);
        mli_String_free(dst);
        return 0;
}

uint64_t mli_String_countn(
        const struct mli_String *str,
        const char match,
        const uint64_t num_chars_to_scan)
{
        uint64_t i = 0;
        uint64_t count = 0u;
        while (i < str->size && i < num_chars_to_scan) {
                if (str->array[i] == match) {
                        count++;
                }
                i++;
        }
        return count;
}

int mli_String_equal_cstr(struct mli_String *self, const char *cstr)
{
        if (self->array == NULL) {
                return 0;
        }
        if (strcmp(self->array, cstr) == 0) {
                return 1;
        } else {
                return 0;
        }
}

int64_t mli_String_compare(
        const struct mli_String *s1,
        const struct mli_String *s2)
{
        return strcmp(s1->array, s2->array);
}

int mli_String_convert_line_break_CRLF_CR_to_LF(
        struct mli_String *dst,
        const struct mli_String *src)
{
        uint64_t i = 0;
        struct mli_IO sdst = mli_IO_init();
        chk(mli_IO_reset(&sdst));

        while (i < src->size) {
                if (mli_cstr_is_CRLF((char *)&src->array[i])) {
                        chk(mli_IO_text_putc(&sdst, '\n'));
                        i += 2;
                } else if (mli_cstr_is_CR((char *)&src->array[i])) {
                        chk(mli_IO_text_putc(&sdst, '\n'));
                        i += 1;
                } else {
                        chk(mli_IO_text_putc(&sdst, src->array[i]));
                        i += 1;
                }
        }

        chk(mli_String_malloc(dst, sdst.size));
        strncpy(dst->array, (char *)sdst.cstr, sdst.size);

        mli_IO_free(&sdst);
        return 1;
chk_error:
        mli_IO_free(&sdst);
        mli_String_free(dst);
        return 0;
}
