/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_triangle_intersection.h"
#include <math.h>
#include "mli_barycentric.h"
#include "mli_math.h"

int mliRay_intersects_triangle(
        const struct mliRay ray,
        const struct mliVec vertex_a,
        const struct mliVec vertex_b,
        const struct mliVec vertex_c,
        double *intersection_ray_parameter)
{
        /* Moeller-Trumbore-intersection-algorithm */
        struct mliVec edge1;
        struct mliVec edge2;
        struct mliVec h, s, q;
        double a, f, u, v, t;
        edge1 = mliVec_substract(vertex_b, vertex_a);
        edge2 = mliVec_substract(vertex_c, vertex_a);
        h = mliVec_cross(ray.direction, edge2);
        a = mliVec_dot(edge1, h);

        if (a > -MLI_EPSILON && a < MLI_EPSILON)
                return 0; /* This ray is parallel to this triangle. */
        f = 1.0 / a;
        s = mliVec_substract(ray.support, vertex_a);
        u = f * mliVec_dot(s, h);
        if (u < 0.0 || u > 1.0)
                return 0;
        q = mliVec_cross(s, edge1);
        v = f * mliVec_dot(ray.direction, q);
        if (v < 0.0 || u + v > 1.0)
                return 0;
        /* At this stage we can compute t to find out where the intersection */
        /* point is on the line. */
        t = f * mliVec_dot(edge2, q);
        if (t > MLI_EPSILON) {
                (*intersection_ray_parameter) = t;
                return 1;
        } else {
                /* This means that there is a line intersection but not a */
                /* ray intersection. */
                return 0;
        }
}

struct mliVec mliTriangle_interpolate_surface_normal(
        const struct mliVec vertex_normal_a,
        const struct mliVec vertex_normal_b,
        const struct mliVec vertex_normal_c,
        const struct mliBarycentrigWeights weights)
{
        return mliVec_set(
                vertex_normal_a.x * weights.a + vertex_normal_b.x * weights.b +
                        vertex_normal_c.x * weights.c,

                vertex_normal_a.y * weights.a + vertex_normal_b.y * weights.b +
                        vertex_normal_c.y * weights.c,

                vertex_normal_a.z * weights.a + vertex_normal_b.z * weights.b +
                        vertex_normal_c.z * weights.c);
}

struct mliVec mliTriangle_surface_normal(
        const struct mliVec vertex_a,
        const struct mliVec vertex_b,
        const struct mliVec vertex_c,
        const struct mliVec vertex_normal_a,
        const struct mliVec vertex_normal_b,
        const struct mliVec vertex_normal_c,
        const struct mliVec intersection_position)
{
        struct mliVec surface_normal;
        struct mliBarycentrigWeights normal_weights = mli_barycentric_weights(
                vertex_a, vertex_b, vertex_c, intersection_position);

        surface_normal = mliTriangle_interpolate_surface_normal(
                vertex_normal_a,
                vertex_normal_b,
                vertex_normal_c,
                normal_weights);

        surface_normal = mliVec_normalized(surface_normal);

        return surface_normal;
}
