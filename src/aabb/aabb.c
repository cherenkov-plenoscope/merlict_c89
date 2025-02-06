/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "aabb.h"
#include "../chk/chk.h"
#include "../math/math.h"
#include "../bool/bool.h"

struct mli_AABB mli_AABB_set(
        const struct mli_Vec lower,
        const struct mli_Vec upper)
{
        struct mli_AABB a;
        a.lower = lower;
        a.upper = upper;
        return a;
}

struct mli_AABB mli_AABB_outermost(
        const struct mli_AABB a,
        const struct mli_AABB b)
{
        struct mli_AABB c;
        c.lower.x = MLI_MATH_MIN2(a.lower.x, b.lower.x);
        c.lower.y = MLI_MATH_MIN2(a.lower.y, b.lower.y);
        c.lower.z = MLI_MATH_MIN2(a.lower.z, b.lower.z);
        c.upper.x = MLI_MATH_MAX2(a.upper.x, b.upper.x);
        c.upper.y = MLI_MATH_MAX2(a.upper.y, b.upper.y);
        c.upper.z = MLI_MATH_MAX2(a.upper.z, b.upper.z);
        return c;
}

struct mli_Vec mli_AABB_center(const struct mli_AABB a)
{
        struct mli_Vec sum = mli_Vec_add(a.upper, a.lower);
        return mli_Vec_multiply(sum, .5);
}

int mli_AABB_valid(const struct mli_AABB a)
{
        chk_msg(!MLI_MATH_IS_NAN(a.lower.x), "aabb.lower.x is 'nan'.");
        chk_msg(!MLI_MATH_IS_NAN(a.lower.y), "aabb.lower.y is 'nan'.");
        chk_msg(!MLI_MATH_IS_NAN(a.lower.z), "aabb.lower.z is 'nan'.");

        chk_msg(!MLI_MATH_IS_NAN(a.upper.x), "aabb.upper.x is 'nan'.");
        chk_msg(!MLI_MATH_IS_NAN(a.upper.y), "aabb.upper.y is 'nan'.");
        chk_msg(!MLI_MATH_IS_NAN(a.upper.z), "aabb.upper.z is 'nan'.");

        chk_msg(a.lower.x <= a.upper.x, "Expected lower.x <= upper.x");
        chk_msg(a.lower.y <= a.upper.y, "Expected lower.y <= upper.y");
        chk_msg(a.lower.z <= a.upper.z, "Expected lower.z <= upper.z");
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

int mli_AABB_equal(const struct mli_AABB a, const struct mli_AABB b)
{
        chk_msg(mli_Vec_equal(a.lower, b.lower),
                "Expected 'lower'-corner to be equal.");
        chk_msg(mli_Vec_equal(a.upper, b.upper),
                "Expected 'upper'-corner to be equal.");
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

int mli_AABB_is_overlapping(const struct mli_AABB a, const struct mli_AABB b)
{
        const int over_x = (a.upper.x >= b.lower.x) && (b.upper.x >= a.lower.x);
        const int over_y = (a.upper.y >= b.lower.y) && (b.upper.y >= a.lower.y);
        const int over_z = (a.upper.z >= b.lower.z) && (b.upper.z >= a.lower.z);
        return (over_x && over_y) && over_z;
}

int mli_AABB_is_point_inside(
        const struct mli_AABB a,
        const struct mli_Vec point)
{
        if (a.lower.x > point.x || a.upper.x <= point.x)
                return MLI_FALSE;
        if (a.lower.y > point.y || a.upper.y <= point.y)
                return MLI_FALSE;
        if (a.lower.z > point.z || a.upper.z <= point.z)
                return MLI_FALSE;
        return MLI_TRUE;
}
