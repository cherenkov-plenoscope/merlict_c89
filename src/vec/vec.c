/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "vec.h"
#include <assert.h>
#include <math.h>
#include <float.h>

struct mli_Vec mli_Vec_init(const double x, const double y, const double z)
{
        struct mli_Vec out;
        out.x = x;
        out.y = y;
        out.z = z;
        return out;
}

struct mli_Vec mli_Vec_add(const struct mli_Vec a, const struct mli_Vec b)
{
        struct mli_Vec out;
        out.x = a.x + b.x;
        out.y = a.y + b.y;
        out.z = a.z + b.z;
        return out;
}

struct mli_Vec mli_Vec_substract(const struct mli_Vec a, const struct mli_Vec b)
{
        struct mli_Vec out;
        out.x = a.x - b.x;
        out.y = a.y - b.y;
        out.z = a.z - b.z;
        return out;
}

struct mli_Vec mli_Vec_cross(const struct mli_Vec a, const struct mli_Vec b)
{
        struct mli_Vec out;
        out.x = (a.y * b.z - a.z * b.y);
        out.y = (a.z * b.x - a.x * b.z);
        out.z = (a.x * b.y - a.y * b.x);
        return out;
}

double mli_Vec_dot(const struct mli_Vec a, const struct mli_Vec b)
{
        return a.x * b.x + a.y * b.y + a.z * b.z;
}

struct mli_Vec mli_Vec_multiply(const struct mli_Vec v, const double a)
{
        struct mli_Vec out;
        out.x = v.x * a;
        out.y = v.y * a;
        out.z = v.z * a;
        return out;
}

double mli_Vec_norm(const struct mli_Vec a) { return sqrt(mli_Vec_dot(a, a)); }

struct mli_Vec mli_Vec_normalized(struct mli_Vec a)
{
        return mli_Vec_multiply(a, 1. / mli_Vec_norm(a));
}

double mli_Vec_angle_between(const struct mli_Vec a, const struct mli_Vec b)
{
        struct mli_Vec a_normalized = mli_Vec_multiply(a, 1. / mli_Vec_norm(a));
        struct mli_Vec b_normalized = mli_Vec_multiply(b, 1. / mli_Vec_norm(b));
        return acos(mli_Vec_dot(a_normalized, b_normalized));
}

double mli_Vec_norm_between(const struct mli_Vec a, const struct mli_Vec b)
{
        return mli_Vec_norm(mli_Vec_substract(a, b));
}

struct mli_Vec mli_Vec_mirror(
        const struct mli_Vec in,
        const struct mli_Vec normal)
{
        /*
         *      This is taken from
         *      (OPTI 421/521 – Introductory Optomechanical Engineering)
         *      J.H. Bruge
         *      University of Arizona
         *
         *                     k1    n     k2
         *                      \    /\   /
         *                       \   |   /
         *                        \  |  /
         *                         \ | /
         *      ____________________\|/______________________
         *      mirror-surface
         *
         *      k1: incidate ray
         *      k2: reflected ray
         *      n:  surface normal
         *
         *      n = [nx,ny,nz]^T
         *
         *      It can be written:
         *
         *      k2 = M*k1
         *
         *      M = EYE - 2*n*n^T
         *
         *      using EYE =  [1 0 0]
         *                   [0 1 0]
         *                   [0 0 1]
         */
        struct mli_Vec out;
        out.x = (1. - 2. * normal.x * normal.x) * in.x +
                -2. * normal.x * normal.y * in.y +
                -2. * normal.x * normal.z * in.z;

        out.y = -2. * normal.x * normal.y * in.x +
                (1. - 2. * normal.y * normal.y) * in.y +
                -2. * normal.y * normal.z * in.z;

        out.z = -2. * normal.x * normal.z * in.x +
                -2. * normal.y * normal.z * in.y +
                (1. - 2. * normal.z * normal.z) * in.z;
        return out;
}

int mli_Vec_equal_margin(
        const struct mli_Vec a,
        const struct mli_Vec b,
        const double distance_margin)
{
        struct mli_Vec diff;
        double distance_squared;
        diff = mli_Vec_substract(a, b);
        distance_squared = mli_Vec_dot(diff, diff);
        return distance_squared <= distance_margin * distance_margin;
}

int mli_Vec_equal(const struct mli_Vec a, const struct mli_Vec b)
{
        if (fabs(a.x - b.x) > DBL_EPSILON)
                return 0;
        if (fabs(a.y - b.y) > DBL_EPSILON)
                return 0;
        if (fabs(a.z - b.z) > DBL_EPSILON)
                return 0;
        return 1;
}

uint32_t mli_Vec_octant(const struct mli_Vec a)
{
        /*
         *  encodes the octant sectors where the vector is pointing to
         *      x y z sector
         *      - - -   0
         *      - - +   1
         *      - + -   2
         *      - + +   3
         *      + - -   4
         *      + - +   5
         *      + + -   6
         *      + + +   7
         */
        const uint32_t sx = a.x >= 0.;
        const uint32_t sy = a.y >= 0.;
        const uint32_t sz = a.z >= 0.;
        return 4 * sx + 2 * sy + 1 * sz;
}

int mli_Vec_sign3_bitmask(const struct mli_Vec a, const double epsilon)
{
        /* bits: 7  6  5  4  3  2  1  0  */
        /*             xp yp zp xn yn zn */

        const int xn = a.x < epsilon ? 4 : 0;   /* 2**2 */
        const int xp = a.x > -epsilon ? 32 : 0; /* 2**5 */

        const int yn = a.y < epsilon ? 2 : 0;   /* 2**1 */
        const int yp = a.y > -epsilon ? 16 : 0; /* 2**4 */

        const int zn = a.z < epsilon ? 1 : 0;  /* 2**0 */
        const int zp = a.z > -epsilon ? 8 : 0; /* 2**3 */

        return (xn | xp | yn | yp | zn | zp);
}

struct mli_Vec mli_Vec_mean(const struct mli_Vec *vecs, const uint64_t num_vecs)
{
        uint64_t i;
        struct mli_Vec mean = mli_Vec_init(0.0, 0.0, 0.0);
        for (i = 0; i < num_vecs; i++) {
                mean = mli_Vec_add(mean, vecs[i]);
        }
        return mli_Vec_multiply(mean, (1.0 / num_vecs));
}

void mli_Vec_set(struct mli_Vec *a, const uint64_t dim, const double v)
{
        switch (dim) {
        case 0:
                a->x = v;
                break;
        case 1:
                a->y = v;
                break;
        case 2:
                a->z = v;
                break;
        default:
                assert(0);
                break;
        }
}

double mli_Vec_get(const struct mli_Vec *a, const uint64_t dim)
{
        double o = 0.0;
        switch (dim) {
        case 0:
                o = a->x;
                break;
        case 1:
                o = a->y;
                break;
        case 2:
                o = a->z;
                break;
        default:
                assert(0);
                break;
        }
        return o;
}
