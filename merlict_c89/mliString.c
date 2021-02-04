/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliString.h"
#include <stdio.h>

struct mliString mliString_init(void)
{
        struct mliString str;
        str.c_str = NULL;
        str.capacity = 0u;
        return str;
}

int mliString_malloc(struct mliString *str, const uint64_t strlen)
{
        mliString_free(str);
        str->capacity = strlen + 1;
        mli_malloc(str->c_str, char, str->capacity);
        memset(str->c_str, '\0', str->capacity);
        return 1;
error:
        mliString_free(str);
        return 0;
}

void mliString_free(struct mliString *str)
{
        free(str->c_str);
        (*str) = mliString_init();
}

int mliString_malloc_from_path(struct mliString *str, const char *path)
{
        uint64_t str_length = 0u;
        FILE *f = fopen(path, "rt");
        mli_check(f != NULL, "Can not read string from path.");
        mli_check(fseek(f, 0, SEEK_END) == 0, "Can not seek to end of file.");
        str_length = ftell(f);
        mli_check(fseek(f, 0, SEEK_SET) == 0, "Can not seek to begin of file");
        mli_check(mliString_malloc(str, str_length), "Can not malloc string.");
        mli_fread(str->c_str, sizeof(char), str_length, f);
        fclose(f);
        return 1;
error:
        if (f != NULL)
                fclose(f);
        mliString_free(str);
        return 0;
}

int mliString_convert_line_break_CRLF_CR_to_LF(
        struct mliString *dst,
        const struct mliString *src)
{
        uint64_t s = 0;
        uint64_t d = 0;

        mli_c(src->capacity > 0);
        mli_c(src->c_str[src->capacity - 1] == '\0');
        mli_c(dst->capacity >= src->capacity);

        while (s < src->capacity) {
                if (_mli_is_CRLF_line_break(&src->c_str[s])) {
                        dst->c_str[d] = '\n';
                        s += 2;
                        d += 1;
                        continue;
                }

                if (_mli_is_CR_line_break(&src->c_str[s])) {
                        dst->c_str[d] = '\n';
                        s += 1;
                        d += 1;
                        continue;
                }

                dst->c_str[d] = src->c_str[s];
                s += 1;
                d += 1;
        }

        while (d < dst->capacity) {
                dst->c_str[d] = '\0';
                d += 1;
        }

        mli_c(dst->c_str[dst->capacity - 1] == '\0');
        return 1;
error:
        return 0;
}
