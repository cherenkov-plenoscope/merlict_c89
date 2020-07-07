/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_variable_length_integer.h"

uint64_t mli_base255_to_decimal(const struct mliDynUint8 *a)
{
        uint64_t i;
        uint64_t prefix = 1;
        uint64_t out = 0;
        for (i = 0; i < a->dyn.size; i++) {
                out += prefix * a->arr[i];
                prefix *= 255;
        }
        return out;
}

int mli_decimal_to_base255(uint64_t in, struct mliDynUint8 *o)
{
        int64_t max_p = 7;
        int64_t i;
        uint64_t prefix = 255u;
        mli_c(mliDynUint8_malloc(o, max_p));
        o->dyn.size = o->dyn.capacity;
        for (i = 0; i < max_p - 1; i++) {
                prefix *= 255u;
        }

        for (i = max_p - 1; i >= 0; i--) {
                prefix /= 255u;
                o->arr[i] = in / prefix;
                in -= o->arr[i] * prefix;
        }
        for (i = o->dyn.capacity - 1; i >= 0; i--) {
                if (o->arr[i] == 0) {
                        o->dyn.size -= 1;
                } else {
                        break;
                }
        }

        return 1;
error:
        return 0;
}

int mliDynUint8_equal(const struct mliDynUint8 *a, const struct mliDynUint8 *b)
{
        if (a->dyn.size > b->dyn.size) {
                return 0;
        } else if (a->dyn.size == b->dyn.size) {
                uint64_t i;
                for (i = 0; i < a->dyn.size; i++) {
                        if (a->arr[i] != b->arr[i]) {
                                return 0;
                        }
                }
                return 1;
        } else {
                return 0;
        }
}

int mliDynUint8_greater_than(
        const struct mliDynUint8 *a,
        const struct mliDynUint8 *b)
{
        if (a->dyn.size > b->dyn.size) {
                return 1;
        } else if (a->dyn.size == b->dyn.size) {
                int64_t i;
                for (i = a->dyn.size - 1; i >= 0; i--) {
                        if (a->arr[i] > b->arr[i]) {
                                return 1;
                        } else if (a->arr[i] < b->arr[i]) {
                                return 0;
                        }
                }
                return 0;
        } else {
                return 0;
        }
}

int mliDynUint8_add(
        const struct mliDynUint8 *a,
        const struct mliDynUint8 *b,
        struct mliDynUint8 *o)
{
        uint64_t i, max_size;
        uint16_t aa, bb, sum, carry, sum8, tmp_o;
        max_size = MLI_MAX2(a->dyn.size, b->dyn.size);
        mli_c(mliDynUint8_malloc(o, max_size + 1));
        o->dyn.size = o->dyn.capacity;
        for (i = 0; i < o->dyn.size; i++) {
                o->arr[i] = 0;
        }
        for (i = 0; i < max_size; i++) {
                aa = i < a->dyn.size ? a->arr[i] : 0;
                bb = i < b->dyn.size ? b->arr[i] : 0;
                sum = aa + bb;
                sum8 = sum % 255;
                carry = sum / 255;
                tmp_o = o->arr[i] + sum8;
                if (tmp_o / 255 > 0) {
                        o->arr[i] = tmp_o % 255;
                        carry += tmp_o / 255;
                } else {
                        o->arr[i] = tmp_o;
                }
                o->arr[i + 1] += carry;
        }
        if (o->dyn.size > 0) {
                if (o->arr[o->dyn.size - 1] == 0) {
                        o->dyn.size -= 1;
                }
        }

        return 1;
error:
        return 0;
}

int mliDynUint8_divide_two(const struct mliDynUint8 *a, struct mliDynUint8 *o)
{
        int64_t i;
        uint64_t val, carry;
        mli_c(mliDynUint8_malloc(o, a->dyn.size));
        o->dyn.size = o->dyn.capacity;
        for (i = 0; i < (int64_t)o->dyn.size; i++) {
                o->arr[i] = 0;
        }
        carry = 0;
        for (i = a->dyn.size - 1; i >= 0; i--) {
                val = a->arr[i] + carry;
                if (val % 2 == 0) {
                        carry = 0;
                } else {
                        carry = 255;
                }
                o->arr[i] = val / 2;
        }
        return 1;
error:
        return 0;
}

int mliDynUint8_malloc_char(struct mliDynUint8 *o, const char *str)
{
        uint64_t i, capacity, length;
        const uint8_t *ptr;
        length = strlen(str);
        capacity = length + 1;
        mli_c(mliDynUint8_malloc(o, capacity));
        o->dyn.size = o->dyn.capacity;
        for (i = 0; i < length; i++) {
                ptr = (uint8_t *)&str[i];
                o->arr[i] = *ptr;
        }
        o->arr[length] = 0;
        return 1;
error:
        return 0;
}
