/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_TRIANGLE_INTERSECTION_H_
#define MLI_TRIANGLE_INTERSECTION_H_

#include "triangle_barycentric.h"
#include "../bool/bool.h"
#include "../vec/vec.h"
#include "../ray/ray.h"

struct mli_Vec mli_Triangle_interpolate_surface_normal(
        const struct mli_Vec vertex_normal_a,
        const struct mli_Vec vertex_normal_b,
        const struct mli_Vec vertex_normal_c,
        const struct mli_triangle_BarycentrigWeights weights);

mli_bool mli_Ray_intersects_triangle(
        const struct mli_Ray ray,
        const struct mli_Vec vertex_a,
        const struct mli_Vec vertex_b,
        const struct mli_Vec vertex_c,
        double *intersection_ray_parameter);

struct mli_Vec mli_Triangle_surface_normal(
        const struct mli_Vec vertex_a,
        const struct mli_Vec vertex_b,
        const struct mli_Vec vertex_c,
        const struct mli_Vec vertex_normal_a,
        const struct mli_Vec vertex_normal_b,
        const struct mli_Vec vertex_normal_c,
        const struct mli_Vec intersection_position);
#endif
