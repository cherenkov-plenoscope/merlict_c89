/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "../color/color.h"
#include "../math/math.h"
#include "../bool/bool.h"

struct mli_Color mli_Color_set(const float r, const float g, const float b)
{
        struct mli_Color rgb;
        rgb.r = r;
        rgb.g = g;
        rgb.b = b;
        return rgb;
}

struct mli_Color mli_Color_mean(
        const struct mli_Color colors[],
        const uint32_t num_colors)
{
        struct mli_Color out = {0., 0., 0.};
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

struct mli_Color mli_Color_truncate(
        const struct mli_Color color,
        const float start,
        const float stop)
{
        struct mli_Color out;
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

mli_bool mli_Color_equal(const struct mli_Color a, const struct mli_Color b)
{
        if (a.r != b.r)
                return MLI_FALSE;
        if (a.g != b.g)
                return MLI_FALSE;
        if (a.b != b.b)
                return MLI_FALSE;
        return MLI_TRUE;
}

mli_bool mli_Color_is_in_range(
        const struct mli_Color c,
        const float start,
        const float stop)
{
        if (MLI_MATH_IS_NAN(c.r))
                return MLI_FALSE;
        if (MLI_MATH_IS_NAN(c.g))
                return MLI_FALSE;
        if (MLI_MATH_IS_NAN(c.b))
                return MLI_FALSE;

        if (c.r < start || c.r >= stop)
                return MLI_FALSE;
        if (c.g < start || c.g >= stop)
                return MLI_FALSE;
        if (c.b < start || c.b >= stop)
                return MLI_FALSE;
        return MLI_TRUE;
}

float mli_Color_luminance(const struct mli_Color self)
{
        return self.r + self.g + self.b;
}

struct mli_Color mli_Color_add(
        const struct mli_Color u,
        const struct mli_Color v)
{
        return mli_Color_set(u.r + v.r, u.g + v.g, u.b + v.b);
}

struct mli_Color mli_Color_multiply(const struct mli_Color c, const double f)
{
        return mli_Color_set(c.r * f, c.g * f, c.b * f);
}
