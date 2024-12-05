/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MTL_CSTR_H_
#define MTL_CSTR_H_

#include <stdint.h>
#include <stdio.h>

int mtl_cstr_ends_with(const char *str, const char *sufix);
int mtl_cstr_starts_with(const char *str, const char *prefix);
int mtl_cstr_has_prefix_suffix(
        const char *str,
        const char *prefix,
        const char *sufix);

int mtl_cstr_split(
        const char *str,
        const char delimiter,
        char *token,
        const uint64_t token_length);
int mtl_cstr_is_CRLF(const char *s);
int mtl_cstr_is_CR(const char *s);
int mtl_cstr_assert_only_NUL_LF_TAB_controls(const char *str);
int mtl_cstr_assert_only_NUL_LF_TAB_controls_dbg(
        const char *str,
        const int dbg);

uint64_t mtl_cstr_count_chars_up_to(
        const char *str,
        const char c,
        const uint64_t num_chars_to_scan);

int mtl_cstr_lines_fprint(
        FILE *f,
        const char *str,
        const uint64_t line,
        const uint64_t line_radius);
void mtl_cstr_path_strip_this_dir(char *dst, const char *src);

void mtl_cstr_path_basename_without_extension(const char *filename, char *key);
void mtl_cstr_strip_spaces(const char *in, char *out);

int mtl_cstr_match_templeate(
        const char *s,
        const char *t,
        const char digit_wildcard);

#endif
