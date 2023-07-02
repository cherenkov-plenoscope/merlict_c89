/* Copyright 2018-2023 Sebastian Achim Mueller */
#ifndef mliStr_H_
#define mliStr_H_
#include <stdint.h>
#include <stdarg.h>

struct mliStr {
        uint64_t length;
        char *cstr;
};

struct mliStr mliStr_init(void);
int mliStr_malloc_copy(struct mliStr *str, const struct mliStr *src);
int mliStr_malloc_copyn(struct mliStr *str, const struct mliStr *src, const uint64_t start, const uint64_t length);
int mliStr_malloc(struct mliStr *str, const uint64_t length);
void mliStr_free(struct mliStr *str);
int mliStr_mallocf(struct mliStr *str, const char *format, ...);
int mliStr_malloc_cstr(struct mliStr *str, const char *s);

int mliStr_ends_with(const struct mliStr *str, const struct mliStr *suffix);
int mliStr_starts_with(const struct mliStr *str, const struct mliStr *prefix);
int mliStr_has_prefix_suffix(
        const struct mliStr *str,
        const struct mliStr *prefix,
        const struct mliStr *suffix);

int64_t mliStr_rfind(const struct mliStr *str, const char c);
int64_t mliStr_find(const struct mliStr *str, const char c);
int mliStr_strip(const struct mliStr *src, struct mliStr *dst);
uint64_t mliStr_countn(
        const struct mliStr *str,
        const char c,
        const uint64_t num_chars_to_scan);

#endif
