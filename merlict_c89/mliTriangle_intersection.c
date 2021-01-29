/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliTriangle_intersection.h"
#include "mli_barycentric.h"


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

struct mliVec mliTriangle_estimate_normal(
        const struct mliVec vertex_normal_a,
        const struct mliVec vertex_normal_b,
        const struct mliVec vertex_normal_c,
        const struct mliBarycentrigWeights weights)
{
        return mliVec_set(
                vertex_normal_a.x * weights.a +
                vertex_normal_b.x * weights.b +
                vertex_normal_c.x * weights.c,

                vertex_normal_a.y * weights.a +
                vertex_normal_b.y * weights.b +
                vertex_normal_c.y * weights.c,

                vertex_normal_a.z * weights.a +
                vertex_normal_b.z * weights.b +
                vertex_normal_c.z * weights.c
        );
}

int mliTriangle_intersection(
        const struct mliRay ray,
        const struct mliVec vertex_a,
        const struct mliVec vertex_b,
        const struct mliVec vertex_c,
        const struct mliVec vertex_normal_a,
        const struct mliVec vertex_normal_b,
        const struct mliVec vertex_normal_c,
        double *intersection_ray_parameter,
        struct mliVec *intersection_position,
        struct mliVec *intersection_normal)
{
        const int valid = mliRay_intersects_triangle(
                ray,
                vertex_a,
                vertex_b,
                vertex_c,
                intersection_ray_parameter);

        if (valid) {
                struct mliBarycentrigWeights normal_weights;
                (*intersection_position) = mliRay_at(
                        &ray,
                        (*intersection_ray_parameter));

                normal_weights = mli_barycentric_weights(
                        vertex_a,
                        vertex_b,
                        vertex_c,
                        (*intersection_position));

                (*intersection_normal) = mliTriangle_estimate_normal(
                        vertex_normal_a,
                        vertex_normal_b,
                        vertex_normal_c,
                        normal_weights
                );

                return 1;
        } else {
                return 0;
        }
}
