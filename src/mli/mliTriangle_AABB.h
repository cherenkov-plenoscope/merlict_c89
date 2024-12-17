/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLITRIANGLE_AABB_H_
#define MLITRIANGLE_AABB_H_

#include <stdint.h>
#include "../vec/vec.h"
#include "../aabb/aabb.h"

struct mliTriangle {
        struct mli_Vec v1;
        struct mli_Vec v2;
        struct mli_Vec v3;
};

struct mli_AABB mliTriangle_aabb(
        const struct mli_Vec a,
        const struct mli_Vec b,
        const struct mli_Vec c);
int mliTriangle_has_overlap_aabb(
        const struct mli_Vec a,
        const struct mli_Vec b,
        const struct mli_Vec c,
        const struct mli_AABB aabb);
struct mliTriangle mliTriangle_set_in_norm_aabb(
        const struct mli_Vec a,
        const struct mli_Vec b,
        const struct mli_Vec c,
        const struct mli_AABB aabb);
int64_t mliTriangle_intersects_norm_aabb(struct mliTriangle t);
int64_t mliTriangle_intersects_point(struct mliTriangle t, struct mli_Vec p);
int64_t mli_triangle_aabb_check_line(
        struct mli_Vec p1,
        struct mli_Vec p2,
        int64_t outcode_diff);
int64_t mli_triangle_aabb_check_point(
        struct mli_Vec p1,
        struct mli_Vec p2,
        double alpha,
        int64_t mask);
int64_t mli_triangle_aabb_bevel_3d(struct mli_Vec p);
int64_t mli_triangle_aabb_bevel_2d(struct mli_Vec p);
int64_t mli_triangle_aabb_face_plane(struct mli_Vec p);
#endif
