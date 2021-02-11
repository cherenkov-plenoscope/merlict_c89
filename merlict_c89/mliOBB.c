/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliOBB.h"
#include "mli_debug.h"

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
        obb.lower.x = MLI_MIN2(a.lower.x, b.lower.x);
        obb.lower.y = MLI_MIN2(a.lower.y, b.lower.y);
        obb.lower.z = MLI_MIN2(a.lower.z, b.lower.z);
        obb.upper.x = MLI_MAX2(a.upper.x, b.upper.x);
        obb.upper.y = MLI_MAX2(a.upper.y, b.upper.y);
        obb.upper.z = MLI_MAX2(a.upper.z, b.upper.z);
        return obb;
}

struct mliVec mliOBB_center(const struct mliOBB a)
{
        struct mliVec sum = mliVec_add(a.upper, a.lower);
        return mliVec_multiply(sum, .5);
}

int mliOBB_valid(const struct mliOBB obb)
{
        mli_check(!MLI_IS_NAN(obb.lower.x), "obb.lower.x is 'nan'.");
        mli_check(!MLI_IS_NAN(obb.lower.y), "obb.lower.y is 'nan'.");
        mli_check(!MLI_IS_NAN(obb.lower.z), "obb.lower.z is 'nan'.");

        mli_check(!MLI_IS_NAN(obb.upper.x), "obb.upper.x is 'nan'.");
        mli_check(!MLI_IS_NAN(obb.upper.y), "obb.upper.y is 'nan'.");
        mli_check(!MLI_IS_NAN(obb.upper.z), "obb.upper.z is 'nan'.");

        mli_check(obb.lower.x <= obb.upper.x, "Expected lower.x <= upper.x");
        mli_check(obb.lower.y <= obb.upper.y, "Expected lower.y <= upper.y");
        mli_check(obb.lower.z <= obb.upper.z, "Expected lower.z <= upper.z");
        return 1;
error:
        return 0;
}

int mliOBB_equal(const struct mliOBB a, const struct mliOBB b)
{
        mli_check(
                mliVec_equal(a.lower, b.lower),
                "Expected 'lower'-corner to be equal.");
        mli_check(
                mliVec_equal(a.upper, b.upper),
                "Expected 'upper'-corner to be equal.");
        return 1;
error:
        return 0;
}
