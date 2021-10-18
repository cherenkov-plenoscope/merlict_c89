/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_CSTR_AND_NUMBERS_H_
#define MLI_CSTR_AND_NUMBERS_H_

#include <stdint.h>

int mli_ncstr_int64(
        int64_t *out,
        const char *s,
        const uint64_t base,
        const uint64_t length);
int mli_cstr_to_int64(int64_t *out, const char *s, const uint64_t base);

int mli_ncstr_to_uint64(
        uint64_t *out,
        const char *s,
        const uint64_t base,
        const uint64_t length);
int mli_cstr_to_uint64(uint64_t *out, const char *s, const uint64_t base);

int mli_ncstr_to_double(double *out, const char *s, const uint64_t length);
int mli_cstr_to_double(double *out, const char *s);

int mli_uint64_to_cstr(
        uint64_t u,
        char *s,
        const uint64_t max_num_chars,
        const uint64_t base,
        const uint64_t min_num_digits);

#endif
