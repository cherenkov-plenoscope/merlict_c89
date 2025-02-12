/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_COLOR_H_
#define MLI_COLOR_H_

#include <stdint.h>
#include "../bool/bool.h"

struct mli_Color {
        float r;
        float g;
        float b;
};

mli_bool mli_Color_equal(const struct mli_Color a, const struct mli_Color b);
mli_bool mli_Color_equal_margin(
        const struct mli_Color a,
        const struct mli_Color b,
        const double epsilon);
struct mli_Color mli_Color_truncate(
        const struct mli_Color color,
        const float start,
        const float stop);
struct mli_Color mli_Color_mean(
        const struct mli_Color colors[],
        const uint32_t num_colors);

struct mli_Color mli_Color_set(const float r, const float g, const float b);
mli_bool mli_Color_is_in_range(
        const struct mli_Color c,
        const float start,
        const float stop);
float mli_Color_luminance(const struct mli_Color self);

struct mli_Color mli_Color_add(
        const struct mli_Color a,
        const struct mli_Color b);
struct mli_Color mli_Color_multiply(const struct mli_Color c, const double f);
#endif
