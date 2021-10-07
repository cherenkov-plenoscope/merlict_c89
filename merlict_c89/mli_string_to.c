/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_string_to.h"
#include <errno.h>
#include <limits.h>
#include <stdlib.h>

int mli_nstring_to_int(
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
        return 1;
error:
        return 0;
}

int mli_nstring_to_uint(
        uint64_t *out,
        const char *s,
        const uint64_t base,
        const uint64_t expected_num_chars)
{
        int64_t tmp;
        chk(mli_nstring_to_int(&tmp, s, base, expected_num_chars));
        chk_msg(tmp >= 0, "Expected a positive integer.");
        (*out) = tmp;
        return 1;
error:
        return 0;
}

int mli_string_to_int(int64_t *out, const char *s, const uint64_t base)
{
        chk_msg(mli_nstring_to_int(out, s, base, strlen(s)),
                "Can not convert string to int64.");
        return 1;
error:
        return 0;
}

int mli_string_to_uint(uint64_t *out, const char *s, const uint64_t base)
{
        int64_t tmp;
        chk(mli_string_to_int(&tmp, s, base));
        chk_msg(tmp >= 0, "Expected a positive integer.");
        (*out) = tmp;
        return 1;
error:
        return 0;
}

int mli_nstring_to_float(
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
        return 1;
error:
        return 0;
}

int mli_string_to_float(double *out, const char *s)
{
        chk_msg(mli_nstring_to_float(out, s, strlen(s)),
                "Can not convert string to float64.");
        return 1;
error:
        return 0;
}

int mli_string_ends_with(const char *str, const char *sufix)
{
        uint64_t len_str, len_sufix;
        if (!str || !sufix) {
                return 0;
        }
        len_str = strlen(str);
        len_sufix = strlen(sufix);
        if (len_sufix > len_str) {
                return 0;
        }
        return strncmp(str + len_str - len_sufix, sufix, len_sufix) == 0;
}

int mli_string_starts_with(const char *str, const char *prefix)
{
        uint64_t len_str, len_prefix;
        if (!str || !prefix) {
                return 0;
        }
        len_str = strlen(str);
        len_prefix = strlen(prefix);
        if (len_prefix > len_str) {
                return 0;
        }
        return strncmp(str, prefix, len_prefix) == 0;
}

int mli_string_has_prefix_suffix(
        const char *str,
        const char *prefix,
        const char *sufix)
{
        uint64_t has_pre = 1;
        uint64_t has_suf = 1;
        if (prefix != NULL) {
                has_pre = mli_string_starts_with(str, prefix);
        }

        if (sufix != NULL) {
                has_suf = mli_string_ends_with(str, sufix);
        }

        if (has_pre == 1 && has_suf == 1) {
                return 1;
        } else {
                return 0;
        }
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
                if (str[i] == '\0') {
                        break;
                } else if (str[i] == delimiter) {
                        break;
                } else {
                        token[i] = str[i];
                }
        }
        return i;
}

int _mli_is_CRLF_line_break(const char *s)
{
        if (s[0] == '\0') {
                return 0;
        }
        if (s[1] == '\0') {
                return 0;
        }
        if (s[0] == '\r' && s[1] == '\n') {
                return 1;
        }
        return 0;
}

int _mli_is_CR_line_break(const char *s)
{
        if (s[0] == '\0') {
                return 0;
        }
        if (s[0] == '\r') {
                return 1;
        }
        return 0;
}

int mli_string_assert_only_NUL_LF_TAB_controls(const char *str)
{
        return mli_string_assert_only_NUL_LF_TAB_controls_dbg(str, 1);
}

int mli_string_assert_only_NUL_LF_TAB_controls_dbg(
        const char *str,
        const int dbg)
{
        uint64_t pos = 0;
        while (str[pos] != '\0') {
                if (str[pos] >= 32 && str[pos] < 127) {
                        /* all fine */
                } else {
                        if (str[pos] == '\n') {
                                /* fine */
                        } else if (str[pos] == '\t') {
                                /* fine */
                        } else {
                                if (dbg) {
                                        fprintf(stderr,
                                                "Control code %u "
                                                "at column %ld in string.\n",
                                                (uint8_t)str[pos],
                                                pos);
                                }
                                return 0;
                        }
                }
                pos += 1;
        }
        return 1;
}

uint64_t mli_string_count_chars_up_to(
        const char *str,
        const char c,
        const uint64_t num_chars_to_scan)
{
        uint64_t i = 0;
        uint64_t count = 0u;
        while (str[i] != '\0' && i < num_chars_to_scan) {
                if (str[i] == c) {
                        count++;
                }
                i++;
        }
        return count;
}

int mli_uint_to_string(
        uint64_t u,
        char *s,
        const uint64_t max_num_chars,
        const uint64_t base,
        const uint64_t min_num_digits)
{
        char literals[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};
        char tmp[128] = {'\0'};
        uint64_t remainder = 0u;
        uint32_t remainder32 = 0u;
        uint64_t quotient = u;
        int64_t digs = 0u;
        int64_t pos = 0;
        int64_t i = 0;
        int64_t num_leading_zeors = 0;

        chk_msg(base <= 10, "Expected base <= 10");
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

        return 1;
error:
        return 0;
}

int _mli_fprint_line_match(
        FILE *f,
        const int64_t line,
        const int64_t line_number)
{
        chk(fprintf(f, "% 6d", (int32_t)line));
        if (line == line_number) {
                chk(fprintf(f, "->|  "));
        } else {
                chk(fprintf(f, "  |  "));
        }
        return 1;
error:
        return 0;
}

int mli_lines_info_fprint(
        FILE *f,
        const char *text,
        const uint64_t line_number,
        const uint64_t line_radius)
{
        int64_t _line_number = (int64_t)line_number;
        int64_t _line_radius = (int64_t)line_radius;
        int64_t line_start = MLI_MAX2(_line_number - _line_radius, 1);
        int64_t line_stop = line_number + line_radius;
        int64_t line = 1;
        int64_t i = 0;

        chk_msg(line_radius > 1, "Expected line_radius > 1.");

        chk(fprintf(f, "  line     text\n"));
        chk(fprintf(f, "        |\n"));

        while (text[i]) {
                int prefix = (line + 1 >= line_start) && (line < line_stop);
                int valid = (line >= line_start) && (line <= line_stop);
                if (text[i] == '\n') {
                        line++;
                }
                if (prefix && i == 0) {
                        chk(_mli_fprint_line_match(f, line, _line_number));
                }
                if (valid) {
                        chk(putc(text[i], f));
                }
                if (prefix && text[i] == '\n') {
                        chk(_mli_fprint_line_match(f, line, _line_number));
                }
                i++;
        }
        chk(putc('\n', f));

        return 1;
error:
        return 0;
}
