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

struct mliOBB mliOBB_dilate(const struct mliOBB a, const double dilation_radius)
{
        struct mliOBB out = a;
        out.lower.x -= dilation_radius;
        out.lower.y -= dilation_radius;
        out.lower.z -= dilation_radius;
        out.upper.x += dilation_radius;
        out.upper.y += dilation_radius;
        out.upper.z += dilation_radius;
        return out;
}

struct mliEdge mliOBB_edge(const struct mliOBB obb, const uint64_t edge_idx)
{
        /*
         *              (l,l,u)         [11]            (l,u,u)
         *                     O---------/------------O
         *                    /.                     /|
         *               [8]-/ .                    / |
         *                  /  .                   /-[10]
         *                 /   .-[4]              /   |-[7]
         *        (u,l,u) O-----------/----------O (u,u,u)
         *                |    .    [9]          |    |
         *      Z         |    .         [3]     |    |
         *      |     (l,l|l)  O- - - - -/- - - -| - -O (l,u,l)
         *      |     [5]-|   .              [6]-|   /
         *      |         |  .                   |  /
         *      /-----Y   | .-[0]                | /-[2]
         *     /          |.                     |/
         *    X           O----------/-----------O
         *        (u,l,l)          [1]            (u,u,l)
         *
         * l: lower
         * u: upper
         *
         */
        struct mliEdge edge;
        const struct mliVec l = obb.lower;
        const struct mliVec u = obb.upper;
        struct mliVec start = mliVec_set(MLI_NAN, MLI_NAN, MLI_NAN);
        struct mliVec stop = mliVec_set(MLI_NAN, MLI_NAN, MLI_NAN);
        switch (edge_idx) {
        case 0:
                start = mliVec_set(l.x, l.y, l.z);
                stop = mliVec_set(u.x, l.y, l.z);
                edge.length = stop.x - start.x;
                break;
        case 1:
                start = mliVec_set(u.x, l.y, l.z);
                stop = mliVec_set(u.x, u.y, l.z);
                edge.length = stop.y - start.y;
                break;
        case 2:
                start = mliVec_set(u.x, u.y, l.z);
                stop = mliVec_set(l.x, u.y, l.z);
                edge.length = stop.x - start.x;
                break;
        case 3:
                start = mliVec_set(l.x, u.y, l.z);
                stop = mliVec_set(l.x, l.y, l.z);
                edge.length = stop.y - start.y;
                break;
        case 4:
                start = mliVec_set(l.x, l.y, l.z);
                stop = mliVec_set(l.x, l.y, u.z);
                edge.length = stop.z - start.z;
                break;
        case 5:
                start = mliVec_set(u.x, l.y, l.z);
                stop = mliVec_set(u.x, l.y, u.z);
                edge.length = stop.z - start.z;
                break;
        case 6:
                start = mliVec_set(u.x, u.y, l.z);
                stop = mliVec_set(u.x, u.y, u.z);
                edge.length = stop.z - start.z;
                break;
        case 7:
                start = mliVec_set(l.x, u.y, l.z);
                stop = mliVec_set(l.x, u.y, u.z);
                edge.length = stop.z - start.z;
                break;
        case 8:
                start = mliVec_set(l.x, l.y, u.z);
                stop = mliVec_set(u.x, l.y, u.z);
                edge.length = stop.x - start.x;
                break;
        case 9:
                start = mliVec_set(u.x, l.y, u.z);
                stop = mliVec_set(u.x, u.y, u.z);
                edge.length = stop.y - start.y;
                break;
        case 10:
                start = mliVec_set(u.x, u.y, u.z);
                stop = mliVec_set(l.x, u.y, u.z);
                edge.length = stop.x - start.x;
                break;
        case 11:
                start = mliVec_set(l.x, u.y, u.z);
                stop = mliVec_set(l.x, l.y, u.z);
                edge.length = stop.y - start.y;
                break;
        default:
                edge.length = MLI_NAN;
                fprintf(stderr, "Unable to determine the edge of a cube.\n");
                assert(0);
                break;
        }
        edge.ray = mliRay_set(start, mliVec_substract(stop, start));
        return edge;
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
