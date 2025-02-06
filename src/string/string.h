/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_STRING_H_
#define MLI_STRING_H_

#include <stdint.h>
#include <stdarg.h>
#include "../chk/chk.h"
#include "../bool/bool.h"
#include "../vector/vector.h"
#include "../array/array.h"

MLI_VECTOR_DEFINITON(mli_String, char)

chk_rc mli_String_from_vargs(
        struct mli_String *self,
        const char *format,
        va_list args);
chk_rc mli_String_from_cstr_fromat(
        struct mli_String *self,
        const char *format,
        ...);
chk_rc mli_String_from_cstr(struct mli_String *self, const char *s);

mli_bool mli_String_equal_cstr(const struct mli_String *self, const char *cstr);

mli_bool mli_String_equal(
        const struct mli_String *self,
        const struct mli_String *other);

mli_bool mli_String_ends_with(
        const struct mli_String *self,
        const struct mli_String *suffix);
mli_bool mli_String_ends_with_cstr(
        const struct mli_String *self,
        const char *cstr);

mli_bool mli_String_starts_with(
        const struct mli_String *self,
        const struct mli_String *prefix);
mli_bool mli_String_starts_with_cstr(
        const struct mli_String *self,
        const char *cstr);

mli_bool mli_String_has_prefix_suffix(
        const struct mli_String *self,
        const struct mli_String *prefix,
        const struct mli_String *suffix);

int64_t mli_String_rfind(const struct mli_String *self, const char c);
int64_t mli_String_find(const struct mli_String *self, const char c);
chk_rc mli_String_strip(const struct mli_String *src, struct mli_String *dst);
uint64_t mli_String_countn(
        const struct mli_String *self,
        const char c,
        const uint64_t num_chars_to_scan);
int64_t mli_String_compare(
        const struct mli_String *s1,
        const struct mli_String *s2);
chk_rc mli_String_convert_line_break_CRLF_CR_to_LF(
        struct mli_String *dst,
        const struct mli_String *src);

int64_t mli_String__discover_size(const struct mli_String *self);
mli_bool mli_String_valid(const struct mli_String *self, const size_t min_size);

chk_rc mli_String__find_idx_with_cstr(
        const struct mli_String *names,
        const uint64_t num_names,
        const char *key,
        uint64_t *idx);

#endif
