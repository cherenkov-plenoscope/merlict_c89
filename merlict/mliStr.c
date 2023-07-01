/* Copyright 2018-2023 Sebastian Achim Mueller */
#include "mliStr.h"
#include <stdio.h>


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
        chk_msg(src->cstr != NULL, "Expctes str to copy from to be allocated");
        mliStr_malloc(str, src->length);
        strncpy(str->cstr, src->cstr, str->length);
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

int mliStr_ends_with(const struct mliStr *str, const struct mliStr *suffix)
{
        if (!str->cstr || !suffix->cstr) {
                return 0;
        }
        if (suffix->length > str->length) {
                return 0;
        }
        return strncmp(
                str->cstr + str->length - suffix->length,
                suffix->cstr,
                suffix->length
        ) == 0;
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
