/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MTL_CSTR_H_
#define MTL_CSTR_H_

#include <stdint.h>
#include <stdio.h>

int mli_cstr_ends_with(const char *str, const char *sufix);
int mli_cstr_starts_with(const char *str, const char *prefix);

int mli_cstr_is_CRLF(const char *s);
int mli_cstr_is_CR(const char *s);
int mli_cstr_assert_only_NUL_LF_TAB_controls(const char *str);
int mli_cstr_assert_only_NUL_LF_TAB_controls_dbg(
        const char *str,
        const int dbg);

int mli_cstr_match_templeate(
        const char *s,
        const char *t,
        const char digit_wildcard);

#endif
