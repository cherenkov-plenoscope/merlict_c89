/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_STRING_TO_H_
#define MERLICT_C89_MLI_STRING_TO_H_

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
int mli_is_CRLF_line_break(const char *s);
int mli_is_CR_line_break(const char *s);
int mli_string_assert_only_NUL_LF_TAB_controls(const char *str);
int mli_string_assert_only_NUL_LF_TAB_controls_dbg(
        const char *str,
        const int dbg);

uint64_t mli_string_count_chars_up_to(
        const char *str,
        const char c,
        const uint64_t num_chars_to_scan);

int mli_lines_info_fprint(
        FILE *f,
        const char *str,
        const uint64_t line,
        const uint64_t line_radius);
void mli_strip_this_dir(char *dst, const char *src);

#endif
