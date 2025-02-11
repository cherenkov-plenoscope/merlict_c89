/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "triangle_intersection.h"
#include <math.h>
#include "../math/math.h"

mli_bool mli_Ray_intersects_triangle(
        const struct mli_Ray ray,
        const struct mli_Vec vertex_a,
        const struct mli_Vec vertex_b,
        const struct mli_Vec vertex_c,
        double *intersection_ray_parameter)
{
        /* Moeller-Trumbore-intersection-algorithm */
        struct mli_Vec edge1;
        struct mli_Vec edge2;
        struct mli_Vec h, s, q;
        double a, f, u, v, t;
        edge1 = mli_Vec_substract(vertex_b, vertex_a);
        edge2 = mli_Vec_substract(vertex_c, vertex_a);
        h = mli_Vec_cross(ray.direction, edge2);
        a = mli_Vec_dot(edge1, h);

        if (a > -MLI_MATH_EPSILON && a < MLI_MATH_EPSILON)
                return MLI_FALSE; /* This ray is parallel to this triangle. */
        f = 1.0 / a;
        s = mli_Vec_substract(ray.support, vertex_a);
        u = f * mli_Vec_dot(s, h);
        if (u < 0.0 || u > 1.0)
                return MLI_FALSE;
        q = mli_Vec_cross(s, edge1);
        v = f * mli_Vec_dot(ray.direction, q);
        if (v < 0.0 || u + v > 1.0)
                return MLI_FALSE;
        /* At this stage we can compute t to find out where the intersection */
        /* point is on the line. */
        t = f * mli_Vec_dot(edge2, q);
        if (t > MLI_MATH_EPSILON) {
                (*intersection_ray_parameter) = t;
                return MLI_TRUE;
        } else {
                /* This means that there is a line intersection but not a */
                /* ray intersection. */
                return MLI_FALSE;
        }
}

struct mli_Vec mli_Triangle_interpolate_surface_normal(
        const struct mli_Vec vertex_normal_a,
        const struct mli_Vec vertex_normal_b,
        const struct mli_Vec vertex_normal_c,
        const struct mli_triangle_BarycentrigWeights weights)
{
        return mli_Vec_set(
                vertex_normal_a.x * weights.a + vertex_normal_b.x * weights.b +
                        vertex_normal_c.x * weights.c,

                vertex_normal_a.y * weights.a + vertex_normal_b.y * weights.b +
                        vertex_normal_c.y * weights.c,

                vertex_normal_a.z * weights.a + vertex_normal_b.z * weights.b +
                        vertex_normal_c.z * weights.c);
}

struct mli_Vec mli_Triangle_surface_normal(
        const struct mli_Vec vertex_a,
        const struct mli_Vec vertex_b,
        const struct mli_Vec vertex_c,
        const struct mli_Vec vertex_normal_a,
        const struct mli_Vec vertex_normal_b,
        const struct mli_Vec vertex_normal_c,
        const struct mli_Vec intersection_position)
{
        struct mli_Vec surface_normal;
        struct mli_triangle_BarycentrigWeights normal_weights =
                mli_triangle_barycentric_weights(
                        vertex_a, vertex_b, vertex_c, intersection_position);

        surface_normal = mli_Triangle_interpolate_surface_normal(
                vertex_normal_a,
                vertex_normal_b,
                vertex_normal_c,
                normal_weights);

        surface_normal = mli_Vec_normalized(surface_normal);

        return surface_normal;
}
