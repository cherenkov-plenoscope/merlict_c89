/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_TRIANGLE_AABB_H_
#define MLI_TRIANGLE_AABB_H_

#include <stdint.h>
#include "triangle.h"
#include "../bool/bool.h"
#include "../vec/vec.h"
#include "../aabb/aabb.h"

struct mli_AABB mli_Triangle_aabb(
        const struct mli_Vec a,
        const struct mli_Vec b,
        const struct mli_Vec c);
mli_bool mli_Triangle_has_overlap_aabb(
        const struct mli_Vec a,
        const struct mli_Vec b,
        const struct mli_Vec c,
        const struct mli_AABB aabb);
struct mli_Triangle mli_Triangle_set_in_norm_aabb(
        const struct mli_Vec a,
        const struct mli_Vec b,
        const struct mli_Vec c,
        const struct mli_AABB aabb);
int64_t mli_Triangle_intersects_norm_aabb(struct mli_Triangle t);
int64_t mli_Triangle_intersects_point(struct mli_Triangle t, struct mli_Vec p);
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
