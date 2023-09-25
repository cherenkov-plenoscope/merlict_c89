/* Copyright 2018-2023 Sebastian Achim Mueller */
#ifndef mliStr_numbers_H_
#define mliStr_numbers_H_
#include <stdint.h>
#include <stdarg.h>
#include "mliStr.h"

int mliStr_nto_double(
        double *out,
        const struct mliStr *str,
        const uint64_t expected_num_chars);
int mliStr_to_double(double *out, const struct mliStr *str);
int mliStr_nto_int64(
        int64_t *out,
        const struct mliStr *str,
        const uint64_t base,
        const uint64_t expected_num_chars);
int mliStr_to_int64(
        int64_t *out,
        const struct mliStr *str,
        const uint64_t base);
int mliStr_nto_uint64(
        uint64_t *out,
        const struct mliStr *str,
        const uint64_t base,
        const uint64_t expected_num_chars);
int mliStr_to_uint64(
        uint64_t *out,
        const struct mliStr *str,
        const uint64_t base);

int mliStr_print_uint64(
        const uint64_t u,
        struct mliStr *str,
        const uint64_t base,
        const uint64_t min_num_digits,
        const char leading_char);

#endif
