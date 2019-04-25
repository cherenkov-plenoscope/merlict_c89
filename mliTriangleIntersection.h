// Copyright 2019 Sebastian Achim Mueller
#ifndef MERLICT_MLITRIANGLEINTERSECTION_H_
#define MERLICT_MLITRIANGLEINTERSECTION_H_
#include <math.h>
#include "mliVec.h"


bool mliRay_intersects_triangle(
    const struct mliVec *support,
    const struct mliVec *direction,
    const struct mliVec *vertex0,
    const struct mliVec *vertex1,
    const struct mliVec *vertex2,
    struct mliVec *out_intersection_point,
    const float EPSILON = 1e-6)
{
    // Moeller-Trumbore-intersection-algorithm
    struct mliVec edge1;
    struct mliVec edge2;
    mliVec_substract(vertex1, vertex0, &edge1);
    mliVec_substract(vertex2, vertex0, &edge2);
    struct mliVec h;
    mliVec_cross(direction, &edge2, &h);
    float a = mliVec_dot(&edge1, &h);

    if (a > -EPSILON && a < EPSILON)
        return false;    // This ray is parallel to this triangle.
    const float f = 1.0/a;
    struct mliVec s;
    mliVec_substract(support, vertex0, &s);
    const float u = f * mliVec_dot(&s, &h);
    if (u < 0.0 || u > 1.0)
        return false;
    struct mliVec q;
    mliVec_cross(&s, &edge1, &q);
    const float v = f * mliVec_dot(direction, &q);
    if (v < 0.0 || u + v > 1.0)
         return false;
    // At this stage we can compute t to find out where the intersection
    // point is on the line.
    const float t = f * mliVec_dot(&edge2, &q);
    if (t > EPSILON) {
        mliRay_pos_at(support, direction, t, out_intersection_point);
        return true;
    } else {
        // This means that there is a line intersection but not a
        // ray intersection.
        return false;
    }
}

#endif  // MERLICT_MLITRIANGLEINTERSECTION_H_
