/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_STRING_TO_H_
#define MERLICT_C89_MLI_STRING_TO_H_

int mli_nstring_to_int(
        int64_t *out,
        const char *s,
        const uint64_t base,
        const uint64_t expected_num_chars);
int mli_string_to_int(int64_t *out, const char *s, const uint64_t base);
int mli_string_to_uint(uint64_t *out, const char *s, const uint64_t base);

int mli_nstring_to_float(
        double *out,
        const char *s,
        const uint64_t expected_num_chars);
int mli_string_to_float(double *out, const char *s);

int mli_string_ends_with(const char *str, const char *sufix);
int mli_string_starts_with(const char *str, const char *prefix);
int mli_string_has_prefix_suffix(
        const char *str,
        const char *prefix,
        const char *sufix);

int mli_string_split(
        const char *str,
        const char delimiter,
        char *token,
        const uint64_t token_length);
int _mli_is_CRLF_line_break(const char *s);
int _mli_is_CR_line_break(const char *s);
int mli_string_assert_only_NUL_LF_TAB_controls(const char *str);

uint64_t mli_string_count_chars_up_to(
        const char *str,
        const char c,
        const uint64_t num_chars_to_scan);

int mli_uint_to_string(
        uint64_t u,
        char *s,
        const uint64_t max_num_chars,
        const uint64_t base,
        const uint64_t min_num_digits);
int mli_lines_info_fprint(
        FILE *f,
        const char *str,
        const uint64_t line,
        const uint64_t line_radius);

#endif
