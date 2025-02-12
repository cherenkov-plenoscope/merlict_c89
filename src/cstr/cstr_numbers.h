/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MTL_CSTR_NUMBERS_H_
#define MTL_CSTR_NUMBERS_H_

#include "../chk/chk.h"
#include <stdint.h>

chk_rc mli_cstr_nto_int64(
        int64_t *out,
        const char *s,
        const uint64_t base,
        const uint64_t length);
chk_rc mli_cstr_to_int64(int64_t *out, const char *s, const uint64_t base);

chk_rc mli_cstr_nto_uint64(
        uint64_t *out,
        const char *s,
        const uint64_t base,
        const uint64_t length);
chk_rc mli_cstr_to_uint64(uint64_t *out, const char *s, const uint64_t base);

chk_rc mli_cstr_nto_double(double *out, const char *s, const uint64_t length);
chk_rc mli_cstr_to_double(double *out, const char *s);

chk_rc mli_cstr_print_uint64(
        uint64_t u,
        char *s,
        const uint64_t max_num_chars,
        const uint64_t base,
        const uint64_t min_num_digits);

#endif
