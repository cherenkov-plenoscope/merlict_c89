/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliOBB.h"

struct mliOBB mliOBB_set(const struct mliVec lower, const struct mliVec upper)
{
        struct mliOBB obb;
        obb.lower = lower;
        obb.upper = upper;
        return obb;
}

struct mliOBB mliOBB_outermost(const struct mliOBB a, const struct mliOBB b)
{
        struct mliOBB obb;
        obb.lower.x = mli_min2(a.lower.x, b.lower.x);
        obb.lower.y = mli_min2(a.lower.y, b.lower.y);
        obb.lower.z = mli_min2(a.lower.z, b.lower.z);
        obb.upper.x = mli_max2(a.upper.x, b.upper.x);
        obb.upper.y = mli_max2(a.upper.y, b.upper.y);
        obb.upper.z = mli_max2(a.upper.z, b.upper.z);
        return obb;
}

struct mliVec mliOBB_center(const struct mliOBB a)
{
        struct mliVec sum = mliVec_add(a.upper, a.lower);
        return mliVec_multiply(sum, .5);
}

int mliOBB_valid(const struct mliOBB obb)
{
        chk_msg(!mli_is_nan(obb.lower.x), "obb.lower.x is 'nan'.");
        chk_msg(!mli_is_nan(obb.lower.y), "obb.lower.y is 'nan'.");
        chk_msg(!mli_is_nan(obb.lower.z), "obb.lower.z is 'nan'.");

        chk_msg(!mli_is_nan(obb.upper.x), "obb.upper.x is 'nan'.");
        chk_msg(!mli_is_nan(obb.upper.y), "obb.upper.y is 'nan'.");
        chk_msg(!mli_is_nan(obb.upper.z), "obb.upper.z is 'nan'.");

        chk_msg(obb.lower.x <= obb.upper.x, "Expected lower.x <= upper.x");
        chk_msg(obb.lower.y <= obb.upper.y, "Expected lower.y <= upper.y");
        chk_msg(obb.lower.z <= obb.upper.z, "Expected lower.z <= upper.z");
        return 1;
error:
        return 0;
}

int mliOBB_equal(const struct mliOBB a, const struct mliOBB b)
{
        chk_msg(mliVec_equal(a.lower, b.lower),
                "Expected 'lower'-corner to be equal.");
        chk_msg(mliVec_equal(a.upper, b.upper),
                "Expected 'upper'-corner to be equal.");
        return 1;
error:
        return 0;
}
