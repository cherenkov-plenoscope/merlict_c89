/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliColor.h"
#include "../../mtl/src/math.h"

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

struct mliColor mliColor_truncate(
        const struct mliColor color,
        const float start,
        const float stop)
{
        struct mliColor out;
        out.r = color.r;
        out.g = color.g;
        out.b = color.b;
        if (out.r > stop)
                out.r = stop;
        if (out.r < start)
                out.r = start;
        if (out.g > stop)
                out.g = stop;
        if (out.g < start)
                out.g = start;
        if (out.b > stop)
                out.b = stop;
        if (out.b < start)
                out.b = start;
        return out;
}

int mliColor_equal(const struct mliColor a, const struct mliColor b)
{
        if (a.r != b.r)
                return 0;
        if (a.g != b.g)
                return 0;
        if (a.b != b.b)
                return 0;
        return 1;
}

int mliColor_is_in_range(
        const struct mliColor c,
        const float start,
        const float stop)
{
        if (MLI_MATH_IS_NAN(c.r))
                return 0;
        if (MLI_MATH_IS_NAN(c.g))
                return 0;
        if (MLI_MATH_IS_NAN(c.b))
                return 0;

        if (c.r < start || c.r >= stop)
                return 0;
        if (c.g < start || c.g >= stop)
                return 0;
        if (c.b < start || c.b >= stop)
                return 0;
        return 1;
}

struct mliColor mliColor_add(const struct mliColor u, const struct mliColor v)
{
        return mliColor_set(u.r + v.r, u.g + v.g, u.b + v.b);
}

struct mliColor mliColor_multiply(const struct mliColor c, const double f)
{
        return mliColor_set(c.r * f, c.g * f, c.b * f);
}

struct mliColor mliColor_multiply_elementwise(
        const struct mliColor u,
        const struct mliColor v)
{
        return mliColor_set(u.r * v.r, u.g * v.g, u.b * v.b);
}
