/* Copyright 2018-2023 Sebastian Achim Mueller */
#ifndef MTL_STRING_NUMBERS_H_
#define MTL_STRING_NUMBERS_H_
#include <stdint.h>
#include <stdarg.h>
#include "string.h"
#include "../chk/chk.h"

chk_rc mli_String_nto_double(
        double *out,
        const struct mli_String *str,
        const uint64_t expected_num_chars);
chk_rc mli_String_to_double(double *out, const struct mli_String *str);
chk_rc mli_String_nto_int64(
        int64_t *out,
        const struct mli_String *str,
        const uint64_t base,
        const uint64_t expected_num_chars);
chk_rc mli_String_to_int64(
        int64_t *out,
        const struct mli_String *str,
        const uint64_t base);
chk_rc mli_String_nto_uint64(
        uint64_t *out,
        const struct mli_String *str,
        const uint64_t base,
        const uint64_t expected_num_chars);
chk_rc mli_String_to_uint64(
        uint64_t *out,
        const struct mli_String *str,
        const uint64_t base);

chk_rc mli_String_print_uint64(
        const uint64_t u,
        struct mli_String *str,
        const uint64_t base,
        const uint64_t min_num_digits,
        const char leading_char);

#endif
