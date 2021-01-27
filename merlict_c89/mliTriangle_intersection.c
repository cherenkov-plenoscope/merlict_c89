/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliTriangle_intersection.h"
#include "mli_barycentric.h"


int mliRay_intersects_triangle(
        const struct mliVec support,
        const struct mliVec direction,
        const struct mliVec vertex_a,
        const struct mliVec vertex_b,
        const struct mliVec vertex_c,
        double *ray_parameter)
{
        /* Moeller-Trumbore-intersection-algorithm */
        struct mliVec edge1;
        struct mliVec edge2;
        struct mliVec h, s, q;
        double a, f, u, v, t;
        edge1 = mliVec_substract(vertex_b, vertex_a);
        edge2 = mliVec_substract(vertex_c, vertex_a);
        h = mliVec_cross(direction, edge2);
        a = mliVec_dot(edge1, h);

        if (a > -MLI_EPSILON && a < MLI_EPSILON)
                return 0; /* This ray is parallel to this triangle. */
        f = 1.0 / a;
        s = mliVec_substract(support, vertex_a);
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

int mliTriangle_intersection(
        const struct mliVec vertex_a,
        const struct mliVec vertex_b,
        const struct mliVec vertex_c,
        const struct mliVec vertex_normal_a,
        const struct mliVec vertex_normal_b,
        const struct mliVec vertex_normal_c,
        const struct mliRay ray,
        struct mliIntersection *intersection)
{
        double ray_parameter;
        int has_intersection = mliRay_intersects_triangle(
                ray.support,
                ray.direction,
                vertex_a,
                vertex_b,
                vertex_c,
                &ray_parameter);

        if (has_intersection) {
                intersection->position_local = mliRay_at(&ray, ray_parameter);
                intersection->surface_normal_local = mli_barycentric(
                        vertex_normal_a,
                        vertex_normal_b,
                        vertex_normal_c,
                        intersection->position);
                intersection->distance_of_ray = ray_parameter;
                intersection->from_outside_to_inside =
                        mli_ray_runs_from_outside_to_inside(
                                ray.direction,
                                intersection->surface_normal_local);
                return 1;
        } else {
                return 0;
        }
}
