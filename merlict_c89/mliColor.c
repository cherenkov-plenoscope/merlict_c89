/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliColor.h"

struct mliColor mliColor_set(const float r, const float g, const float b)
{
        struct mliColor rgb;
        rgb.r = r;
        rgb.g = g;
        rgb.b = b;
        return rgb;
}

struct mliColor mliColor_mix(
        const struct mliColor a,
        const struct mliColor b,
        const float refl)
{
        struct mliColor out;
        out.r = (1.f - refl) * a.r + refl * b.r;
        out.g = (1.f - refl) * a.g + refl * b.g;
        out.b = (1.f - refl) * a.b + refl * b.b;
        return out;
}

struct mliColor mliColor_mean(
        const struct mliColor colors[],
        const uint32_t num_colors)
{
        struct mliColor out = {0., 0., 0.};
        const float f_num_colors = (float)num_colors;
        uint32_t i;
        for (i = 0; i < num_colors; i++) {
                out.r = out.r + colors[i].r;
                out.g = out.g + colors[i].g;
                out.b = out.b + colors[i].b;
        }
        out.r = out.r / f_num_colors;
        out.g = out.g / f_num_colors;
        out.b = out.b / f_num_colors;
        return out;
}

struct mliColor mliColor_truncate_to_uint8(const struct mliColor color)
{
        struct mliColor out;
        out.r = color.r;
        out.g = color.g;
        out.b = color.b;
        if (out.r > 255.)
                out.r = 255.;
        if (out.r < 0.)
                out.r = 0.;
        if (out.g > 255.)
                out.g = 255.;
        if (out.g < 0.)
                out.g = 0.;
        if (out.b > 255.)
                out.b = 255.;
        if (out.b < 0.)
                out.b = 0.;
        return out;
}

int mliColor_is_equal(const struct mliColor a, const struct mliColor b)
{
        if (a.r != b.r)
                return 0;
        if (a.g != b.g)
                return 0;
        if (a.b != b.b)
                return 0;
        return 1;
}

int mliColor_is_valid_8bit_range(const struct mliColor c)
{
        if (MLI_IS_NAN(c.r))
                return 0;
        if (MLI_IS_NAN(c.g))
                return 0;
        if (MLI_IS_NAN(c.b))
                return 0;

        if (c.r < 0.0 || c.r >= 256.0)
                return 0;
        if (c.g < 0.0 || c.g >= 256.0)
                return 0;
        if (c.b < 0.0 || c.b >= 256.0)
                return 0;
        return 1;
}
