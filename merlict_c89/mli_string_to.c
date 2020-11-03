/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_string_to.h"

int mli_string_to_int(int64_t *out, char *s, uint64_t base)
{
        char *end;
        int64_t l;
        mli_check(
                !(s[0] == '\0' || isspace(s[0])),
                "Can not convert string to int64, bad string.");
        errno = 0;
        l = strtol(s, &end, base);
        mli_check(
                errno != ERANGE,
                "Can not convert string to int64, over-, under-flow.");
        mli_check(*end == '\0', "Can not convert string to int64, bad string.");
        *out = l;
        return 1;
error:
        return 0;
}

int mli_string_to_float(double *out, char *s)
{
        char *end;
        double l;
        mli_check(
                !(s[0] == '\0' || isspace(s[0])),
                "Can not convert string to float64, bad string.");
        errno = 0;
        l = strtod(s, &end);
        mli_check(
                errno != ERANGE,
                "Can not convert string to float64, over-, under-flow.");
        mli_check(
                *end == '\0', "Can not convert string to float64, bad string.");
        *out = l;
        return 1;
error:
        return 0;
}

int mli_string_ends_with(const char *str, const char *suffix)
{
        uint64_t len_str, len_suffix;
        if (!str || !suffix) {
                return 0;
        }
        len_str = strlen(str);
        len_suffix = strlen(suffix);
        if (len_suffix > len_str) {
                return 0;
        }
        return strncmp(str + len_str - len_suffix, suffix, len_suffix) == 0;
}

int mli_string_split(
        const char *str,
        const char delimiter,
        char *token,
        const uint64_t token_length)
{
        uint64_t i = 0;
        memset(token, '\0', token_length);
        for (i = 0; i < token_length; i++) {
                if (str[i] == delimiter) {
                        i ++;
                        break;
                } else if (str[i] == '\0') {
                        break;
                } else {
                        token[i] = str[i];
                }
        }
        return i;
}

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

int mliString_malloc_from_file(struct mliString *str, const char *path)
{
        uint64_t str_length = 0u;
        FILE *f = fopen(path, "rt");
        mli_check(f != NULL, "Can not read string from path.");
        mli_check(fseek(f, 0, SEEK_END) == 0, "Can not seek to end of file.");
        str_length = ftell(f);
        mli_check(fseek(f, 0, SEEK_SET) == 0, "Can not seek to begin of file");
        mli_check(
                mliString_malloc(str, str_length),
                "Can not malloc string."
        );
        mli_fread(str->c_str, sizeof(char), str_length, f);
        fclose(f);
        return 1;
error:
        if (f != NULL)
                fclose(f);
        mliString_free(str);
        return 0;
}
