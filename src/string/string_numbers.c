/* Copyright 2018-2023 Sebastian Achim Mueller */
#include "string_numbers.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "../chk/chk.h"

int mli_String_nto_double(
        double *out,
        const struct mli_String *str,
        const uint64_t expected_num_chars)
{
        char *end;
        uint64_t actual_num_chars = 0u;
        double l;
        chk_msg(str->array != NULL, "Expected str to be allocated.");
        chk_msg(!(str->array[0] == '\0' || isspace(str->array[0])),
                "Can not convert string to double, bad string.");
        errno = 0;
        l = strtod(str->array, &end);
        chk_msg(errno != ERANGE,
                "Can not convert string to double, over-, under-flow.");
        chk_msg(end != NULL, "Can not convert string to double.");

        actual_num_chars = end - str->array;
        chk_msg(actual_num_chars == expected_num_chars,
                "double has not the expected number of chars.");
        *out = l;
        return 1;
chk_error:
        return 0;
}

int mli_String_to_double(double *out, const struct mli_String *str)
{
        chk_msg(mli_String_nto_double(out, str, str->size),
                "Can not convert mli_String to double.");
        return 1;
chk_error:
        return 0;
}

int mli_String_nto_int64(
        int64_t *out,
        const struct mli_String *str,
        const uint64_t base,
        const uint64_t expected_num_chars)
{
        char *end;
        uint64_t actual_num_chars = 0u;
        int64_t l;
        chk_msg(str->array != NULL, "Expected str to be allocated.");
        chk_msg(!(str->array[0] == '\0' || isspace(str->array[0])),
                "Can not convert string to int64, bad string.");
        errno = 0;
        l = strtol(str->array, &end, base);
        chk_msg(errno != ERANGE,
                "Can not convert string to int64, over-, under-flow.");
        chk_msg(end != NULL, "Can not convert string to int64, bad string.");
        actual_num_chars = end - str->array;
        chk_msg(actual_num_chars == expected_num_chars,
                "Integer has not the expected number of chars.");
        *out = l;
        return 1;
chk_error:
        return 0;
}

int mli_String_to_int64(
        int64_t *out,
        const struct mli_String *str,
        const uint64_t base)
{
        chk_msg(mli_String_nto_int64(out, str, base, str->size),
                "Can not convert string to int64.");
        return 1;
chk_error:
        return 0;
}

int mli_String_nto_uint64(
        uint64_t *out,
        const struct mli_String *str,
        const uint64_t base,
        const uint64_t expected_num_chars)
{
        int64_t tmp;
        chk(mli_String_nto_int64(&tmp, str, base, expected_num_chars));
        chk_msg(tmp >= 0, "Expected a positive integer.");
        (*out) = tmp;
        return 1;
chk_error:
        return 0;
}

int mli_String_to_uint64(
        uint64_t *out,
        const struct mli_String *str,
        const uint64_t base)
{
        int64_t tmp;
        chk(mli_String_to_int64(&tmp, str, base));
        chk_msg(tmp >= 0, "Expected a positive integer.");
        (*out) = tmp;
        return 1;
chk_error:
        return 0;
}

int mli_String_reverse_print_uint64(
        const uint64_t u,
        struct mli_String *str,
        const uint64_t base)
{
        char literals[] = {
                '0',
                '1',
                '2',
                '3',
                '4',
                '5',
                '6',
                '7',
                '8',
                '9',
                'A',
                'B',
                'C',
                'D',
                'E',
                'F'};
        char tmp[128] = {'\0'};
        uint64_t remainder = 0u;
        uint32_t remainder32 = 0u;
        uint64_t quotient = u;
        int64_t digs = 0u;

        chk_msg(base <= 16, "Expected base <= 16");
        chk_msg(base > 1, "Expected base > 1");
        mli_String_free(str);

        do {
                remainder = quotient % base;
                quotient = quotient / base;
                remainder32 = (uint32_t)remainder;
                tmp[digs] = literals[remainder32];
                digs++;
                chk_msg(digs < 127, "Exceeded max_num_chars.");
        } while (quotient > 0u);

        chk(mli_String_malloc(str, digs));
        strncpy(str->array, tmp, digs);
        return 1;
chk_error:
        mli_String_free(str);
        return 0;
}

int mli_String_print_uint64(
        const uint64_t u,
        struct mli_String *str,
        const uint64_t base,
        const uint64_t min_num_digits,
        const char leading_char)
{
        struct mli_String tmp = mli_String_init();
        int64_t pos = 0;
        int64_t i = 0;
        int64_t length = 0;
        int64_t num_leading = 0;
        int64_t MAX_NUM_CHARS = 128;

        chk_msg(base <= 16, "Expected base <= 16");
        chk_msg(base > 1, "Expected base > 1");

        chk(mli_String_reverse_print_uint64(u, &tmp, base));

        num_leading = min_num_digits - tmp.size;
        if (num_leading < 0) {
                num_leading = 0;
        }
        length = num_leading + tmp.size;
        chk(mli_String_malloc(str, length));
        chk_msg(length < MAX_NUM_CHARS, "Exceeded max_num_chars.");

        pos = 0;
        for (i = 0; i < num_leading; i++) {
                str->array[pos] = leading_char;
                pos++;
        }

        for (i = 0; i < (int64_t)tmp.size; i++) {
                str->array[pos] = tmp.array[(int64_t)tmp.size - i - 1];
                pos++;
        }

        mli_String_free(&tmp);
        return 1;
chk_error:
        mli_String_free(&tmp);
        return 0;
}
