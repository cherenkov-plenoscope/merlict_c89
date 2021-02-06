/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLITRIANGLE_INTERSECTION_H_
#define MERLICT_C89_MLITRIANGLE_INTERSECTION_H_

#include "mliVec.h"
#include "mliRay.h"

int mliTriangle_intersection(
        const struct mliRay ray,
        const struct mliVec vertex_a,
        const struct mliVec vertex_b,
        const struct mliVec vertex_c,
        double *intersection_ray_parameter,
        struct mliVec *intersection_position);

struct mliVec mliTriangle_interpolate_surface_normal(
        const struct mliVec vertex_normal_a,
        const struct mliVec vertex_normal_b,
        const struct mliVec vertex_normal_c,
        const struct mliBarycentrigWeights weights);

int mliRay_intersects_triangle(
        const struct mliRay ray,
        const struct mliVec vertex_a,
        const struct mliVec vertex_b,
        const struct mliVec vertex_c,
        double *intersection_ray_parameter);

struct mliVec mliTriangle_surface_normal(
        const struct mliVec vertex_a,
        const struct mliVec vertex_b,
        const struct mliVec vertex_c,
        const struct mliVec vertex_normal_a,
        const struct mliVec vertex_normal_b,
        const struct mliVec vertex_normal_c,
        const struct mliVec intersection_position);
#endif
