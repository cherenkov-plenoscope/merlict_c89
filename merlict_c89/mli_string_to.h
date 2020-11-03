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
#endif
