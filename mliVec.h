/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIVEC_H_
#define MERLICT_MLIVEC_H_

#include <math.h>
#include <float.h>
#include <stdint.h>

struct mliVec{
        double x;
        double y;
        double z;
};

struct mliVec mliVec_set(const double x, const double y, const double z)
{
        struct mliVec out;
        out.x = x;
        out.y = y;
        out.z = z;
        return out;
}

struct mliVec mliVec_add(const struct mliVec a, const struct mliVec b)
{
        struct mliVec out;
        out.x = a.x + b.x;
        out.y = a.y + b.y;
        out.z = a.z + b.z;
        return out;
}

struct mliVec mliVec_substract(const struct mliVec a, const struct mliVec b)
{
        struct mliVec out;
        out.x = a.x - b.x;
        out.y = a.y - b.y;
        out.z = a.z - b.z;
        return out;
}

struct mliVec mliVec_cross(const struct mliVec a, const struct mliVec b)
{
        struct mliVec out;
        out.x = (a.y*b.z - a.z*b.y);
        out.y = (a.z*b.x - a.x*b.z);
        out.z = (a.x*b.y - a.y*b.x);
        return out;
}

double mliVec_dot(const struct mliVec a, const struct mliVec b)
{
        return a.x*b.x + a.y*b.y + a.z*b.z;
}

struct mliVec mliVec_multiply(const struct mliVec v, const double a)
{
        struct mliVec out;
        out.x = v.x*a;
        out.y = v.y*a;
        out.z = v.z*a;
        return out;
}

double mliVec_norm(const struct mliVec a)
{
        return sqrt(mliVec_dot(a, a));
}

struct mliVec mliVec_normalized(struct mliVec a)
{
        return mliVec_multiply(a, 1./mliVec_norm(a));
}

double mliVec_angle_between(const struct mliVec a, const struct mliVec b)
{
        struct mliVec a_normalized = mliVec_multiply(a, 1./mliVec_norm(a));
        struct mliVec b_normalized = mliVec_multiply(b, 1./mliVec_norm(b));
        return acos(mliVec_dot(a_normalized, b_normalized));
}

struct mliVec mliVec_mirror(const struct mliVec in, const struct mliVec normal)
{
        /*  This is taken from
         *      (OPTI 421/521 – Introductory Optomechanical Engineering)
         *      J.H. Bruge
         *      University of Arizona
         *
         *                                 k1    n     k2
         *                                      \    /\   /
         *                                       \   |   /
         *                                        \  |  /
         *                                         \ | /
         *      ____________________\|/______________________
         *       mirror-surface
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
         *                               [0 1 0]
         *                               [0 0 1]
         */
        struct mliVec out;
        out.x = (1. - 2.*normal.x*normal.x) * in.x +
                    - 2.*normal.x*normal.y  * in.y +
                    - 2.*normal.x*normal.z  * in.z;

        out.y =     - 2.*normal.x*normal.y  * in.x +
                (1. - 2.*normal.y*normal.y) * in.y +
                    - 2.*normal.y*normal.z  * in.z;

        out.z =     - 2.*normal.x*normal.z  * in.x +
                    - 2.*normal.y*normal.z  * in.y +
                (1. - 2.*normal.z*normal.z) * in.z;
        return out;
}

int mliVec_equal_margin(
        const struct mliVec a,
        const struct mliVec b,
        const double distance_margin)
{
        struct mliVec diff;
        double distance_squared;
        diff = mliVec_substract(a, b);
        distance_squared = mliVec_dot(diff, diff);
        return distance_squared <= distance_margin*distance_margin;
}

int mliVec_is_equal(const struct mliVec a, const struct mliVec b)
{
        if (fabs(a.x - b.x) > DBL_EPSILON) return 0;
        if (fabs(a.y - b.y) > DBL_EPSILON) return 0;
        if (fabs(a.z - b.z) > DBL_EPSILON) return 0;
        return 1;
}

uint32_t mliVec_octant(const struct mliVec a)
{
        /*  encodes the octant sectors where the vector is pointing to
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
        return 4*sx + 2*sy + 1*sz;
}

void mliVec_ncpy(
        const struct mliVec *from,
        struct mliVec* to,
        const uint64_t num)
{
        uint64_t i;
        for (i = 0; i < num; i++) {
                to[i] = from[i];
        }
}

void mliVec_print(const struct mliVec v)
{
        printf("[%.2f, %.2f, %.2f]", v.x, v.y, v.z);
}

#endif
