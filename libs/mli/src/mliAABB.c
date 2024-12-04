/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliAABB.h"
#include "../../chk/src/chk.h"
#include "mli_math.h"

struct mliAABB mliAABB_set(const struct mliVec lower, const struct mliVec upper)
{
        struct mliAABB a;
        a.lower = lower;
        a.upper = upper;
        return a;
}

struct mliAABB mliAABB_outermost(const struct mliAABB a, const struct mliAABB b)
{
        struct mliAABB c;
        c.lower.x = MLI_MATH_MIN2(a.lower.x, b.lower.x);
        c.lower.y = MLI_MATH_MIN2(a.lower.y, b.lower.y);
        c.lower.z = MLI_MATH_MIN2(a.lower.z, b.lower.z);
        c.upper.x = MLI_MATH_MAX2(a.upper.x, b.upper.x);
        c.upper.y = MLI_MATH_MAX2(a.upper.y, b.upper.y);
        c.upper.z = MLI_MATH_MAX2(a.upper.z, b.upper.z);
        return c;
}

struct mliVec mliAABB_center(const struct mliAABB a)
{
        struct mliVec sum = mliVec_add(a.upper, a.lower);
        return mliVec_multiply(sum, .5);
}

int mliAABB_valid(const struct mliAABB a)
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
        return 1;
chk_error:
        return 0;
}

int mliAABB_equal(const struct mliAABB a, const struct mliAABB b)
{
        chk_msg(mliVec_equal(a.lower, b.lower),
                "Expected 'lower'-corner to be equal.");
        chk_msg(mliVec_equal(a.upper, b.upper),
                "Expected 'upper'-corner to be equal.");
        return 1;
chk_error:
        return 0;
}

int mliAABB_is_overlapping(const struct mliAABB a, const struct mliAABB b)
{
        const int over_x = (a.upper.x >= b.lower.x) && (b.upper.x >= a.lower.x);
        const int over_y = (a.upper.y >= b.lower.y) && (b.upper.y >= a.lower.y);
        const int over_z = (a.upper.z >= b.lower.z) && (b.upper.z >= a.lower.z);
        return (over_x && over_y) && over_z;
}

int mliAABB_is_point_inside(const struct mliAABB a, const struct mliVec point)
{
        if (a.lower.x > point.x || a.upper.x <= point.x)
                return 0;
        if (a.lower.y > point.y || a.upper.y <= point.y)
                return 0;
        if (a.lower.z > point.z || a.upper.z <= point.z)
                return 0;
        return 1;
}
