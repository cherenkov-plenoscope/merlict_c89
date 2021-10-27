/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliStr.h"
#include <stdio.h>

struct mliStr mliStr_init(void)
{
        struct mliStr str;
        str.capacity = 0u;
        str.length = 0u;
        str.c_str = NULL;
        return str;
}

void mliStr_free(struct mliStr *str)
{
        free(str->c_str);
        (*str) = mliStr_init();
}

int mliStr_malloc(struct mliStr *str, const uint64_t capacity)
{
        mliStr_free(str);
        str->capacity = MLI_MAX2(2, capacity);
        str->length = 0u;
        chk_malloc(str->c_str, char, str->capacity);
        memset(str->c_str, '\0', str->capacity);
        return 1;
error:
        return 0;
}

int mliStr_push_back_char(struct mliStr *str, const char c)
{
        const uint64_t new_length = str->length + 1;

        if (new_length >= str->capacity) {
                const uint64_t min_new_capacity =
                        MLI_MAX2(new_length, 2 * str->capacity);

                str->capacity = min_new_capacity * 2;
                str->c_str = (char *)realloc(
                        (void *)str->c_str, str->capacity * sizeof(char));
                memset(&str->c_str[str->length],
                       '\0',
                       str->capacity - str->length);
                chk_mem(str->c_str);
        }
        str->c_str[str->length] = c;
        str->length = new_length;

        return 1;
error:
        return 0;
}

int mliStr_push_back_c_str(struct mliStr *str, const char *s)
{
        const uint64_t slen = strlen(s);
        uint64_t i;
        for (i = 0; i < slen; i++) {
                chk_msg(mliStr_push_back_char(str, s[i]),
                        "Failed to push back char");
        }
        return 1;
error:
        return 0;
}

int mliStr_malloc_from_path(struct mliStr *str, const char *path)
{
        int c = EOF;
        FILE *f = fopen(path, "rt");
        chk_msg(f, "Failed to open file.");
        chk_msg(mliStr_malloc(str, 0), "Can not malloc string.");
        c = getc(f);
        while (c != EOF) {
                chk_msg(mliStr_push_back_char(str, c),
                        "Failed to push back char.");
                c = getc(f);
        }
        fclose(f);
        return 1;
error:
        if (f != NULL)
                fclose(f);
        mliStr_free(str);
        return 0;
}

int mliStr_convert_line_break_CRLF_CR_to_LF(
        struct mliStr *dst,
        const struct mliStr *src)
{
        uint64_t i = 0;
        mliStr_free(dst);
        chk(mliStr_malloc(dst, src->capacity));

        while (i < src->capacity) {
                if (mli_cstr_is_CRLF(&src->c_str[i])) {
                        chk(mliStr_push_back_char(dst, '\n'));
                        i += 2;
                } else if (mli_cstr_is_CR(&src->c_str[i])) {
                        chk(mliStr_push_back_char(dst, '\n'));
                        i += 1;
                } else {
                        chk(mliStr_push_back_char(dst, src->c_str[i]));
                        i += 1;
                }
        }

        return 1;
error:
        return 0;
}
