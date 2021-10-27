/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLICOLOR_H_
#define MLICOLOR_H_

#include <stdint.h>

struct mliColor {
        float r;
        float g;
        float b;
};

int mliColor_equal(const struct mliColor a, const struct mliColor b);
struct mliColor mliColor_truncate_to_uint8(const struct mliColor color);
struct mliColor mliColor_mean(
        const struct mliColor colors[],
        const uint32_t num_colors);
struct mliColor mliColor_mix(
        const struct mliColor a,
        const struct mliColor b,
        const float refl);
struct mliColor mliColor_set(const float r, const float g, const float b);
int mliColor_is_valid_8bit_range(const struct mliColor c);

struct mliColor mliColor_add(const struct mliColor a, const struct mliColor b);
struct mliColor mliColor_multiply(const struct mliColor c, const double f);
struct mliColor mliColor_multiply_elementwise(
        const struct mliColor a,
        const struct mliColor b);
#endif
