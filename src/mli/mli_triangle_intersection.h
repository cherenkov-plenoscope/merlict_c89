/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLITRIANGLE_INTERSECTION_H_
#define MLITRIANGLE_INTERSECTION_H_

#include "../vec/vec.h"
#include "../ray/ray.h"
#include "../triangle/triangle_barycentric.h"

struct mli_Vec mliTriangle_interpolate_surface_normal(
        const struct mli_Vec vertex_normal_a,
        const struct mli_Vec vertex_normal_b,
        const struct mli_Vec vertex_normal_c,
        const struct mliBarycentrigWeights weights);

int mli_Ray_intersects_triangle(
        const struct mli_Ray ray,
        const struct mli_Vec vertex_a,
        const struct mli_Vec vertex_b,
        const struct mli_Vec vertex_c,
        double *intersection_ray_parameter);

struct mli_Vec mliTriangle_surface_normal(
        const struct mli_Vec vertex_a,
        const struct mli_Vec vertex_b,
        const struct mli_Vec vertex_c,
        const struct mli_Vec vertex_normal_a,
        const struct mli_Vec vertex_normal_b,
        const struct mli_Vec vertex_normal_c,
        const struct mli_Vec intersection_position);
#endif
