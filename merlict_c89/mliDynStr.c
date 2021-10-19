/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliDynStr.h"
#include <stdio.h>

struct mliDynStr mliDynStr_init(void)
{
        struct mliDynStr str;
        str.capacity = 0u;
        str.length = 0u;
        str.c_str = NULL;
        return str;
}

void mliDynStr_free(struct mliDynStr *str)
{
        free(str->c_str);
        (*str) = mliDynStr_init();
}

int mliDynStr_malloc(struct mliDynStr *str, const uint64_t capacity)
{
        mliDynStr_free(str);
        str->capacity = MLI_MAX2(2, capacity);
        str->length = 0u;
        chk_malloc(str->c_str, char, str->capacity);
        memset(str->c_str, '\0', str->capacity);
        return 1;
error:
        return 0;
}

int mliDynStr_push_back_char(struct mliDynStr *str, const char c)
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

int mliDynStr_push_back_c_str(struct mliDynStr *str, const char *s)
{
        const uint64_t slen = strlen(s);
        uint64_t i;
        for (i = 0; i < slen; i++) {
                chk_msg(mliDynStr_push_back_char(str, s[i]),
                        "Failed to push back char");
        }
        return 1;
error:
        return 0;
}

int mliDynStr_malloc_from_path(struct mliDynStr *str, const char *path)
{
        int c = EOF;
        FILE *f = fopen(path, "rt");
        chk_msg(f, "Failed to open file.");
        chk_msg(mliDynStr_malloc(str, 0), "Can not malloc string.");
        c = getc(f);
        while (c != EOF) {
                chk_msg(mliDynStr_push_back_char(str, c),
                        "Failed to push back char.");
                c = getc(f);
        }
        fclose(f);
        return 1;
error:
        if (f != NULL)
                fclose(f);
        mliDynStr_free(str);
        return 0;
}

int mliDynStr_convert_line_break_CRLF_CR_to_LF(
        struct mliDynStr *dst,
        const struct mliDynStr *src)
{
        uint64_t i = 0;
        mliDynStr_free(dst);
        chk(mliDynStr_malloc(dst, src->capacity));

        while (i < src->capacity) {
                if (_mli_is_CRLF_line_break(&src->c_str[i])) {
                        chk(mliDynStr_push_back_char(dst, '\n'));
                        i += 2;
                } else if (_mli_is_CR_line_break(&src->c_str[i])) {
                        chk(mliDynStr_push_back_char(dst, '\n'));
                        i += 1;
                } else {
                        chk(mliDynStr_push_back_char(dst, src->c_str[i]));
                        i += 1;
                }
        }

        return 1;
error:
        return 0;
}
