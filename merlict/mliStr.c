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

int mliStr_mallocf(struct mliStr *str, const char *format, ...)
{
        struct mliStr tmp = mliStr_init();
        va_list args;

        chk(mliStr_malloc(&tmp, 10 * strlen(format)));
        va_start(args, format);
        vsprintf(tmp.cstr, format, args);
        chk(mliStr_malloc(str, strlen(tmp.cstr)));
        strcpy(str->cstr, tmp.cstr);
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
