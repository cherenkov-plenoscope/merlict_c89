/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIVEC_H_
#define MERLICT_MLIVEC_H_

#include <math.h>
#include <stdint.h>

typedef struct {
    double x;
    double y;
    double z;
} mliVec;

mliVec mliVec_set(
    const double x,
    const double y,
    const double z) {
    mliVec out;
    out.x = x;
    out.y = y;
    out.z = z;
    return out;}

mliVec mliVec_add(
    const mliVec a,
    const mliVec b) {
    mliVec out;
    out.x = a.x + b.x;
    out.y = a.y + b.y;
    out.z = a.z + b.z;
    return out;}

mliVec mliVec_substract(
    const mliVec a,
    const mliVec b) {
    mliVec out;
    out.x = a.x - b.x;
    out.y = a.y - b.y;
    out.z = a.z - b.z;
    return out;}

mliVec mliVec_cross(
    const mliVec a,
    const mliVec b) {
    mliVec out;
    out.x = (a.y*b.z - a.z*b.y);
    out.y = (a.z*b.x - a.x*b.z);
    out.z = (a.x*b.y - a.y*b.x);
    return out;}

double mliVec_dot(
    const mliVec a,
    const mliVec b) {
    return a.x*b.x + a.y*b.y + a.z*b.z;}

mliVec mliVec_multiply(const mliVec v, const double a) {
    mliVec out;
    out.x = v.x*a;
    out.y = v.y*a;
    out.z = v.z*a;
    return out;}

double mliVec_norm(
    const mliVec a) {
    return sqrt(mliVec_dot(a, a));}

mliVec mliVec_normalized(mliVec a) {
    return mliVec_multiply(a, 1./mliVec_norm(a));}

double mliVec_angle_between(const mliVec a, const mliVec b) {
    mliVec a_normalized = mliVec_multiply(a, 1./mliVec_norm(a));
    mliVec b_normalized = mliVec_multiply(b, 1./mliVec_norm(b));
    return acos(mliVec_dot(a_normalized, b_normalized));}

mliVec mliVec_mirror(
    const mliVec in,
    const mliVec surface_normal) {
    /*  This is taken from
        (OPTI 421/521 – Introductory Optomechanical Engineering)
        J.H. Bruge
        University of Arizona

                       k1    n     k2
                        \    /\   /
                         \   |   /
                          \  |  /
                           \ | /
        ____________________\|/______________________
         mirror-surface

        k1: incidate ray
        k2: reflected ray
        n:  surface normal

        n = [nx,ny,nz]^T

        It can be written:

        k2 = M*k1

        M = EYE - 2*n*n^T

        using EYE =  [1 0 0]
                     [0 1 0]
                     [0 0 1]
    */
    mliVec out;
    out.x = (1.  - 2.*surface_normal.x*surface_normal.x) * in.x +
                 - 2.*surface_normal.x*surface_normal.y  * in.y +
                 - 2.*surface_normal.x*surface_normal.z  * in.z;

    out.y =      - 2.*surface_normal.x*surface_normal.y  * in.x +
             (1. - 2.*surface_normal.y*surface_normal.y) * in.y +
                 - 2.*surface_normal.y*surface_normal.z  * in.z;

    out.z =     -  2.*surface_normal.x*surface_normal.z  * in.x +
                 - 2.*surface_normal.y*surface_normal.z  * in.y +
             (1. - 2.*surface_normal.z*surface_normal.z) * in.z;
    return out;
}

int mliVec_equal_margin(
    const mliVec a,
    const mliVec b,
    const double distance_margin) {
    mliVec diff;
    double distance_squared;
    diff = mliVec_substract(a, b);
    distance_squared = mliVec_dot(diff, diff);
    return distance_squared <= distance_margin*distance_margin;}

int mliVec_is_equal(const mliVec a, const mliVec b) {
    if (a.x != b.x) return 0;
    if (a.y != b.y) return 0;
    if (a.z != b.z) return 0;
    return 1;}

uint32_t mliVec_octant(const mliVec a) {
    /*  encodes the octant sectors where the vector is pointing to
        x y z sector
        - - -   0
        - - +   1
        - + -   2
        - + +   3
        + - -   4
        + - +   5
        + + -   6
        + + +   7
    */
    const uint32_t sx = a.x >= 0.;
    const uint32_t sy = a.y >= 0.;
    const uint32_t sz = a.z >= 0.;
    return 4*sx + 2*sy + 1*sz;}

void mliVec_ncpy(
    const mliVec *from,
    mliVec* to,
    const uint64_t num) {
    uint64_t i;
    for (i = 0; i < num; i++) {
        to[i] = from[i];}}

#endif
