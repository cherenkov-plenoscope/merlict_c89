/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLITRIANGLEINTERSECTION_H_
#define MERLICT_MLITRIANGLEINTERSECTION_H_
#include <math.h>
#include "mli_math.h"
#include "mliVec.h"
#include "mliRay.h"
#include "mliIntersection.h"
#include "mli_from_outside_to_inside.h"


int mliRay_intersects_triangle(
    const struct mliVec support,
    const struct mliVec direction,
    const struct mliVec vertex0,
    const struct mliVec vertex1,
    const struct mliVec vertex2,
    double *ray_parameter)
{
    /* Moeller-Trumbore-intersection-algorithm */
    struct mliVec edge1;
    struct mliVec edge2;
    struct mliVec h, s, q;
    double a, f, u, v, t;
    edge1 = mliVec_substract(vertex1, vertex0);
    edge2 = mliVec_substract(vertex2, vertex0);
    h = mliVec_cross(direction, edge2);
    a = mliVec_dot(edge1, h);

    if (a > -MLI_EPSILON && a < MLI_EPSILON)
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
    if (t > MLI_EPSILON) {
        (*ray_parameter) = t;
        return 1;
    } else {
        /* This means that there is a line intersection but not a */
        /* ray intersection. */
        return 0;
    }
}

struct mliVec mli_triangle_surface_normal(
    const struct mliVec vertex0,
    const struct mliVec vertex1,
    const struct mliVec vertex2) {
    struct mliVec edge1 = mliVec_substract(vertex1, vertex0);
    struct mliVec edge2 = mliVec_substract(vertex2, vertex0);
    return mliVec_cross(edge1, edge2);}


int mliTriangle_intersection(
    const struct mliVec vertex0,
    const struct mliVec vertex1,
    const struct mliVec vertex2,
    const struct mliRay ray,
    struct mliIntersection *intersection) {
    double ray_parameter;
    if (mliRay_intersects_triangle(
            ray.support,
            ray.direction,
            vertex0,
            vertex1,
            vertex2,
            &ray_parameter)
    ) {
        intersection->position = mliRay_at(&ray, ray_parameter);
        intersection->surface_normal = mli_triangle_surface_normal(
            vertex0,
            vertex1,
            vertex2);
        intersection->distance_of_ray = ray_parameter;
        intersection->from_outside_to_inside =
            mli_ray_runs_from_outside_to_inside(
                ray.direction,
                intersection->surface_normal);
        return 1;
    } else {
        return 0;
    }
}

#endif
