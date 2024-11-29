/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MTL_STRING_H_
#define MTL_STRING_H_

#include "array.h"
#include <stdarg.h>

MTL_ARRAY_DEFINITON(mtl_String, char)

int mtl_String_mallocf(struct mtl_String *str, const char *format, ...);
int mtl_String_malloc_cstr(struct mtl_String *str, const char *s);

int mtl_String_ends_with(
        const struct mtl_String *str,
        const struct mtl_String *suffix);
int mtl_String_starts_with(
        const struct mtl_String *str,
        const struct mtl_String *prefix);
int mtl_String_has_prefix_suffix(
        const struct mtl_String *str,
        const struct mtl_String *prefix,
        const struct mtl_String *suffix);

int64_t mtl_String_rfind(const struct mtl_String *str, const char c);
int64_t mtl_String_find(const struct mtl_String *str, const char c);
int mtl_String_strip(const struct mtl_String *src, struct mtl_String *dst);
uint64_t mtl_String_countn(
        const struct mtl_String *str,
        const char c,
        const uint64_t num_chars_to_scan);

#endif
