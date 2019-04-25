/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIVEC_H_
#define MERLICT_MLIVEC_H_

#include <math.h>
#include <stdint.h>

typedef struct {
    float x;
    float y;
    float z;
} mliVec;

void mliVec_add(
    const mliVec *a,
    const mliVec *b,
    mliVec *out) {
    out->x = a->x + b->x;
    out->y = a->y + b->y;
    out->z = a->z + b->z;}

void mliVec_substract(
    const mliVec *a,
    const mliVec *b,
    mliVec *out) {
    out->x = a->x - b->x;
    out->y = a->y - b->y;
    out->z = a->z - b->z;}

void mliVec_cross(
    const mliVec *a,
    const mliVec *b,
    mliVec *out) {
    out->x = (a->y*b->z - a->z*b->y);
    out->y = (a->z*b->x - a->x*b->z);
    out->z = (a->x*b->y - a->y*b->x);}

float mliVec_dot(
    const mliVec *a,
    const mliVec *b) {
    return a->x*b->x + a->y*b->y + a->z*b->z;}

float mliVec_norm(
    const mliVec *a) {
    return sqrt(mliVec_dot(a, a));}

void mliVec_mirror(
    const mliVec *in,
    const mliVec *surface_normal,
    mliVec *out) {
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
    out->x = (1. - 2.*surface_normal->x*surface_normal->x) * in->x +
                 - 2.*surface_normal->x*surface_normal->y  * in->y +
                 - 2.*surface_normal->x*surface_normal->z  * in->z;

    out->y =     - 2.*surface_normal->x*surface_normal->y  * in->x +
             (1. - 2.*surface_normal->y*surface_normal->y) * in->y +
                 - 2.*surface_normal->y*surface_normal->z  * in->z;

    out->z =     - 2.*surface_normal->x*surface_normal->z  * in->x +
                 - 2.*surface_normal->y*surface_normal->z  * in->y +
             (1. - 2.*surface_normal->z*surface_normal->z) * in->z;
}

int mliVec_equal_margin(
    const mliVec *a,
    const mliVec *b,
    const float distance_margin) {
    mliVec diff;
    float distance_squared;
    mliVec_substract(a, b, &diff);
    distance_squared = mliVec_dot(&diff, &diff);
    return distance_squared <= distance_margin*distance_margin;}

uint32_t mliVec_octant(const mliVec *a) {
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
    const uint32_t sx = a->x >= 0.;
    const uint32_t sy = a->y >= 0.;
    const uint32_t sz = a->z >= 0.;
    return 4*sx + 2*sy + 1*sz;}

void mliRay_pos_at(
    const mliVec *support,
    const mliVec *direction,
    const float t,
    mliVec* out) {
    out->x = support->x + t *direction->x;
    out->y = support->y + t *direction->y;
    out->z = support->z + t *direction->z;}

#endif
