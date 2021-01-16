/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_STRING_TO_H_
#define MERLICT_C89_MLI_STRING_TO_H_

#include <errno.h>
#include <limits.h>
#include <stdlib.h>

#include "mli_debug.h"

int mli_string_ends_with(const char *str, const char *suffix);
int mli_string_to_float(double *out, char *s);
int mli_string_to_int(int64_t *out, char *s, uint64_t base);
int mli_string_split(
        const char *str,
        const char delimiter,
        char *token,
        const uint64_t token_length);

struct mliString {
        char *c_str;
        uint64_t capacity;
};

struct mliString mliString_init(void);
int mliString_malloc(struct mliString *str, const uint64_t strlen);
void mliString_free(struct mliString *str);
int mliString_malloc_from_file(struct mliString *str, const char *path);

#endif
