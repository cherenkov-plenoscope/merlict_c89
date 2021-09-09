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
        mli_malloc(str->c_str, char, str->capacity);
        memset(str->c_str, '\0', str->capacity);
        return 1;
error:
        return 0;
}

int mliDynStr_push_back(struct mliDynStr *str, const char *s)
{
        const uint64_t slen = strlen(s);
        const uint64_t new_size = str->length + slen;

        if (new_size >= str->capacity) {
                const uint64_t min_new_capacity =
                        MLI_MAX2(new_size, 2 * str->capacity);

                str->capacity = min_new_capacity * 2;
                str->c_str = (char *)realloc(
                        (void *)str->c_str, str->capacity * sizeof(char));
                memset(&str->c_str[str->length],
                       '\0',
                       str->capacity - str->length);
                mli_check_mem(str->c_str);
        }

        memcpy(&str->c_str[str->length], &s[0], slen);
        str->length += slen;

        return 1;
error:
        return 0;
}

int mliDynStr_malloc_from_path(struct mliDynStr *str, const char *path)
{
        uint64_t str_length = 0u;
        uint64_t str_capacity = 0u;
        FILE *f = fopen(path, "rt");
        mli_check(f != NULL, "Can not read string from path.");
        mli_check(fseek(f, 0, SEEK_END) == 0, "Can not seek to end of file.");
        str_length = ftell(f);
        str_capacity = str_length + 1;
        mli_check(fseek(f, 0, SEEK_SET) == 0, "Can not seek to begin of file");
        mli_check(
                mliDynStr_malloc(str, str_capacity), "Can not malloc string.");
        mli_check_fread(str->c_str, sizeof(char), str_length, f);
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
        char buff[2];
        uint64_t i = 0;
        mliDynStr_free(dst);
        mli_c(mliDynStr_malloc(dst, src->capacity));

        while (i < src->capacity) {
                if (_mli_is_CRLF_line_break(&src->c_str[i])) {
                        mli_c(mliDynStr_push_back(dst, "\n"));
                        i += 2;
                } else if (_mli_is_CR_line_break(&src->c_str[i])) {
                        mli_c(mliDynStr_push_back(dst, "\n"));
                        i += 1;
                } else {
                        buff[0] = src->c_str[i];
                        buff[1] = '\0';
                        mli_c(mliDynStr_push_back(dst, buff));
                        i += 1;
                }
        }

        return 1;
error:
        return 0;
}
