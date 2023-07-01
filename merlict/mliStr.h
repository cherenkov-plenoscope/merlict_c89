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
int mliStr_malloc(struct mliStr *byt, const uint64_t length);
void mliStr_free(struct mliStr *str);
int mliStr_mallocf(struct mliStr *str, const char *format, ...);

int mliStr_ends_with(const struct mliStr *str, const struct mliStr *suffix);
int mliStr_starts_with(const struct mliStr *str, const struct mliStr *prefix);
int mliStr_has_prefix_suffix(
        const struct mliStr *str,
        const struct mliStr *prefix,
        const struct mliStr *suffix);

#endif
