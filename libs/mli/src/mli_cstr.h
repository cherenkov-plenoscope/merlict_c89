/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_CSTR_H_
#define MLI_CSTR_H_

#include <stdint.h>
#include <stdio.h>

int mli_cstr_ends_with(const char *str, const char *sufix);
int mli_cstr_starts_with(const char *str, const char *prefix);
int mli_cstr_has_prefix_suffix(
        const char *str,
        const char *prefix,
        const char *sufix);

int mli_cstr_split(
        const char *str,
        const char delimiter,
        char *token,
        const uint64_t token_length);
int mli_cstr_is_CRLF(const char *s);
int mli_cstr_is_CR(const char *s);
int mli_cstr_assert_only_NUL_LF_TAB_controls(const char *str);
int mli_cstr_assert_only_NUL_LF_TAB_controls_dbg(
        const char *str,
        const int dbg);

uint64_t mli_cstr_count_chars_up_to(
        const char *str,
        const char c,
        const uint64_t num_chars_to_scan);

int mli_cstr_lines_fprint(
        FILE *f,
        const char *str,
        const uint64_t line,
        const uint64_t line_radius);
void mli_cstr_path_strip_this_dir(char *dst, const char *src);

void mli_cstr_path_basename_without_extension(const char *filename, char *key);
void mli_cstr_strip_spaces(const char *in, char *out);

int mli_cstr_match_templeate(
        const char *s,
        const char *t,
        const char digit_wildcard);

#endif
