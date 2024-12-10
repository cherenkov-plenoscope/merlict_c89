/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_STRING_H_
#define MLI_STRING_H_

#include "../vector/vector.h"
#include "../array/array.h"
#include <stdarg.h>

MLI_VECTOR_DEFINITON(mli_String, char)

int mli_String_from_cstr_fromat(
        struct mli_String *str,
        const char *format,
        ...);
int mli_String_from_cstr(struct mli_String *str, const char *s);

int mli_String_equal_cstr(struct mli_String *self, const char *cstr);

int mli_String_ends_with(
        const struct mli_String *str,
        const struct mli_String *suffix);
int mli_String_starts_with(
        const struct mli_String *str,
        const struct mli_String *prefix);
int mli_String_has_prefix_suffix(
        const struct mli_String *str,
        const struct mli_String *prefix,
        const struct mli_String *suffix);

int64_t mli_String_rfind(const struct mli_String *str, const char c);
int64_t mli_String_find(const struct mli_String *str, const char c);
int mli_String_strip(const struct mli_String *src, struct mli_String *dst);
uint64_t mli_String_countn(
        const struct mli_String *str,
        const char c,
        const uint64_t num_chars_to_scan);
int64_t mli_String_compare(
        const struct mli_String *s1,
        const struct mli_String *s2);
int mli_String_convert_line_break_CRLF_CR_to_LF(
        struct mli_String *dst,
        const struct mli_String *src);

int64_t mli_String__discover_length(struct mli_String *self);

#endif
