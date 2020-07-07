/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_VARIABLE_LENGTH_INTEGER_H_
#define MERLICT_C89_MLI_VARIABLE_LENGTH_INTEGER_H_

#include <stdint.h>

#include "mli_debug.h"
#include "mliDynArray_template.h"

int mliDynUint8_malloc_char(struct mliDynUint8 *o, const char *str);
int mliDynUint8_divide_two(const struct mliDynUint8 *a, struct mliDynUint8 *o);
int mliDynUint8_add(
        const struct mliDynUint8 *a,
        const struct mliDynUint8 *b,
        struct mliDynUint8 *o);
int mliDynUint8_greater_than(
        const struct mliDynUint8 *a,
        const struct mliDynUint8 *b);
int mliDynUint8_equal(const struct mliDynUint8 *a, const struct mliDynUint8 *b);
int mli_decimal_to_base255(uint64_t in, struct mliDynUint8 *o);
uint64_t mli_base255_to_decimal(const struct mliDynUint8 *a);
#endif
