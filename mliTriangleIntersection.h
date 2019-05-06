/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLITRIANGLEINTERSECTION_H_
#define MERLICT_MLITRIANGLEINTERSECTION_H_
#include <math.h>
#include "mliVec.h"


int mliRay_intersects_triangle(
    const mliVec support,
    const mliVec direction,
    const mliVec vertex0,
    const mliVec vertex1,
    const mliVec vertex2,
    double *ray_parameter)
{
    const float EPSILON = 1e-6;
    /* Moeller-Trumbore-intersection-algorithm */
    mliVec edge1;
    mliVec edge2;
    mliVec h, s, q;
    float a, f, u, v, t;
    edge1 = mliVec_substract(vertex1, vertex0);
    edge2 = mliVec_substract(vertex2, vertex0);
    h = mliVec_cross(direction, edge2);
    a = mliVec_dot(edge1, h);

    if (a > -EPSILON && a < EPSILON)
        return 0;    /* This ray is parallel to this triangle. */
    f = 1.0/a;
    s = mliVec_substract(support, vertex0);
    u = f * mliVec_dot(s, h);
    if (u < 0.0 || u > 1.0)
        return 0;
    q = mliVec_cross(s, edge1);
    v = f * mliVec_dot(direction, q);
    if (v < 0.0 || u + v > 1.0)
         return 0;
    /* At this stage we can compute t to find out where the intersection */
    /* point is on the line. */
    t = f * mliVec_dot(edge2, q);
    if (t > EPSILON) {
        (*ray_parameter) = t;
        return 1;
    } else {
        /* This means that there is a line intersection but not a */
        /* ray intersection. */
        return 0;
    }
}

mliVec mli_triangle_surface_normal(
    const mliVec vertex0,
    const mliVec vertex1,
    const mliVec vertex2) {
    mliVec edge1 = mliVec_substract(vertex1, vertex0);
    mliVec edge2 = mliVec_substract(vertex2, vertex0);
    return mliVec_cross(edge1, edge2);}

#endif
