/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "cstr_numbers.h"
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <limits.h>
#include <stdlib.h>
#include "../chk/chk.h"

chk_rc mli_cstr_nto_int64(
        int64_t *out,
        const char *s,
        const uint64_t base,
        const uint64_t expected_num_chars)
{
        char *end;
        uint64_t actual_num_chars = 0u;
        int64_t l;
        chk_msg(!(s[0] == '\0' || isspace(s[0])),
                "Can not convert string to int64, bad string.");
        errno = 0;
        l = strtol(s, &end, base);
        chk_msg(errno != ERANGE,
                "Can not convert string to int64, over-, under-flow.");
        chk_msg(end != NULL, "Can not convert string to int64, bad string.");
        actual_num_chars = end - s;
        chk_msg(actual_num_chars == expected_num_chars,
                "Integer has not the expected number of chars.");
        *out = l;
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_cstr_to_int64(int64_t *out, const char *s, const uint64_t base)
{
        chk_msg(mli_cstr_nto_int64(out, s, base, strlen(s)),
                "Can not convert string to int64.");
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_cstr_nto_uint64(
        uint64_t *out,
        const char *s,
        const uint64_t base,
        const uint64_t expected_num_chars)
{
        int64_t tmp;
        chk(mli_cstr_nto_int64(&tmp, s, base, expected_num_chars));
        chk_msg(tmp >= 0, "Expected a positive integer.");
        (*out) = tmp;
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_cstr_to_uint64(uint64_t *out, const char *s, const uint64_t base)
{
        int64_t tmp;
        chk(mli_cstr_to_int64(&tmp, s, base));
        chk_msg(tmp >= 0, "Expected a positive integer.");
        (*out) = tmp;
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_cstr_nto_double(
        double *out,
        const char *s,
        const uint64_t expected_num_chars)
{
        char *end;
        uint64_t actual_num_chars = 0u;
        double l;
        chk_msg(!(s[0] == '\0' || isspace(s[0])),
                "Can not convert string to float64, bad string.");
        errno = 0;
        l = strtod(s, &end);
        chk_msg(errno != ERANGE,
                "Can not convert string to float64, over-, under-flow.");
        chk_msg(end != NULL, "Can not convert string to float64.");

        actual_num_chars = end - s;
        chk_msg(actual_num_chars == expected_num_chars,
                "float64 has not the expected number of chars.");
        *out = l;
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_cstr_to_double(double *out, const char *s)
{
        chk_msg(mli_cstr_nto_double(out, s, strlen(s)),
                "Can not convert string to float64.");
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_cstr_print_uint64(
        uint64_t u,
        char *s,
        const uint64_t max_num_chars,
        const uint64_t base,
        const uint64_t min_num_digits)
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
        int64_t pos = 0;
        int64_t i = 0;
        int64_t num_leading_zeors = 0;

        chk_msg(base <= 16, "Expected base <= 16");
        chk_msg(base > 1, "Expected base > 1");
        chk_msg(max_num_chars < sizeof(tmp), "Exceeded max num. chars.");
        chk_msg(min_num_digits < max_num_chars, "Exceeded max num. chars.");

        do {
                remainder = quotient % base;
                quotient = quotient / base;
                remainder32 = (uint32_t)remainder;
                tmp[digs] = literals[remainder32];
                digs++;
                chk_msg(digs < (int64_t)sizeof(tmp),
                        "Exceeded max num. chars.");
        } while (quotient > 0u);

        num_leading_zeors = min_num_digits - digs;
        if (num_leading_zeors < 0) {
                num_leading_zeors = 0;
        }

        for (i = 0; i < num_leading_zeors; i++) {
                chk_msg(pos < (int64_t)max_num_chars,
                        "Exceeded max num. chars.");
                s[pos] = '0';
                pos++;
        }

        for (i = 0; i < digs; i++) {
                chk_msg(pos < (int64_t)max_num_chars,
                        "Exceeded max num. chars.");
                s[pos] = tmp[digs - i - 1];
                pos++;
        }

        chk_msg(pos < (int64_t)max_num_chars, "Exceeded max num. chars.");
        s[pos] = '\0';

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}
