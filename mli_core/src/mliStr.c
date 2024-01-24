/* Copyright 2018-2023 Sebastian Achim Mueller */
#include "mliStr.h"
#include <stdio.h>
#include <stdlib.h>
#include "../../chk_debug/src/chk_debug.h"

struct mliStr mliStr_init(void)
{
        struct mliStr str;
        str.length = 0u;
        str.cstr = NULL;
        return str;
}

void mliStr_free(struct mliStr *str)
{
        free(str->cstr);
        (*str) = mliStr_init();
}

int mliStr_malloc(struct mliStr *str, const uint64_t length)
{
        mliStr_free(str);
        str->length = length;
        chk_malloc(str->cstr, char, str->length + 1);
        memset(str->cstr, '\0', str->length + 1);
        return 1;
error:
        return 0;
}

int mliStr_malloc_copy(struct mliStr *str, const struct mliStr *src)
{
        return mliStr_malloc_copyn(str, src, 0, src->length);
}

int mliStr_malloc_copyn(
        struct mliStr *str,
        const struct mliStr *src,
        const uint64_t start,
        const uint64_t length)
{
        chk_msg(src->cstr != NULL, "Expected src to be allocated");
        chk_msg(start + length <= src->length,
                "Expected start + length < src->length.")
                mliStr_malloc(str, length);
        strncpy(str->cstr, &src->cstr[start], length);
        return 1;
error:
        return 0;
}

int mliStr_mallocf(struct mliStr *str, const char *format, ...)
{
        struct mliStr tmp = mliStr_init();
        va_list args;

        chk(mliStr_malloc(&tmp, 10 * strlen(format)));
        va_start(args, format);
        vsprintf(tmp.cstr, format, args);
        chk(mliStr_malloc(str, strlen(tmp.cstr)));
        strncpy(str->cstr, tmp.cstr, str->length);
        va_end(args);
        mliStr_free(&tmp);
        return 1;
error:
        va_end(args);
        mliStr_free(&tmp);
        return 0;
}

int mliStr_malloc_cstr(struct mliStr *str, const char *s)
{
        chk(mliStr_malloc(str, strlen(s)));
        strncpy(str->cstr, s, str->length);
        return 1;
error:
        return 0;
}

int mliStr_ends_with(const struct mliStr *str, const struct mliStr *suffix)
{
        if (!str->cstr || !suffix->cstr) {
                return 0;
        }
        if (suffix->length > str->length) {
                return 0;
        }
        return strncmp(str->cstr + str->length - suffix->length,
                       suffix->cstr,
                       suffix->length) == 0;
}

int mliStr_starts_with(const struct mliStr *str, const struct mliStr *prefix)
{
        if (!str->cstr || !prefix->cstr) {
                return 0;
        }
        if (prefix->length > str->length) {
                return 0;
        }
        return strncmp(str->cstr, prefix->cstr, prefix->length) == 0;
}

int mliStr_has_prefix_suffix(
        const struct mliStr *str,
        const struct mliStr *prefix,
        const struct mliStr *suffix)
{
        uint64_t has_pre = 1;
        uint64_t has_suf = 1;
        if (prefix->cstr != NULL) {
                has_pre = mliStr_starts_with(str, prefix);
        }

        if (suffix->cstr != NULL) {
                has_suf = mliStr_ends_with(str, suffix);
        }

        if (has_pre == 1 && has_suf == 1) {
                return 1;
        } else {
                return 0;
        }
}

int64_t mliStr_rfind(const struct mliStr *str, const char c)
{
        int64_t i;
        for (i = str->length - 1; i >= 0; i--) {
                if (str->cstr[i] == '\0') {
                        return -1;
                } else if (str->cstr[i] == c) {
                        return i;
                }
        }
        return -1;
}

int64_t mliStr_find(const struct mliStr *str, const char c)
{
        int64_t i;
        for (i = 0; i < (int64_t)str->length; i++) {
                if (str->cstr[i] == '\0') {
                        return -1;
                } else if (str->cstr[i] == c) {
                        return i;
                }
        }
        return -1;
}

int mliStr_match_templeate(
        const struct mliStr *s,
        const struct mliStr *t,
        const char digit_wildcard)
{
        uint64_t i;
        if (s->length != t->length) {
                return 0;
        }
        for (i = 0; i < s->length; i++) {
                if (t->cstr[i] == digit_wildcard) {
                        if (!isdigit(s->cstr[i])) {
                                return 0;
                        }
                } else {
                        if (s->cstr[i] != t->cstr[i]) {
                                return 0;
                        }
                }
        }
        return 1;
}

int mliStr_strip(const struct mliStr *src, struct mliStr *dst)
{
        int64_t start = 0;
        int64_t stop = 0;
        int64_t len = -1;
        struct mliStr cpysrc = mliStr_init();

        chk_msg(src->cstr, "Expected src-string to be allocated.");
        chk_msg(mliStr_malloc_copy(&cpysrc, src), "Can not copy input.");
        mliStr_free(dst);

        while (start < (int64_t)cpysrc.length && isspace(cpysrc.cstr[start])) {
                start += 1;
        }

        stop = cpysrc.length - 1;
        while (stop >= 0 && isspace(cpysrc.cstr[stop])) {
                stop -= 1;
        }

        len = stop - start;

        if (len < 0) {
                chk(mliStr_mallocf(dst, ""));
        } else {
                chk(mliStr_malloc(dst, len + 1));
                strncpy(dst->cstr, &cpysrc.cstr[start], len + 1);
        }
        mliStr_free(&cpysrc);
        return 1;
error:
        mliStr_free(&cpysrc);
        mliStr_free(dst);
        return 0;
}

uint64_t mliStr_countn(
        const struct mliStr *str,
        const char match,
        const uint64_t num_chars_to_scan)
{
        uint64_t i = 0;
        uint64_t count = 0u;
        while (i < str->length && i < num_chars_to_scan) {
                if (str->cstr[i] == match) {
                        count++;
                }
                i++;
        }
        return count;
}
