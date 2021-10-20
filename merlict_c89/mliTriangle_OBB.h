/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLITRIANGLE_OBB_H_
#define MERLICT_C89_MLITRIANGLE_OBB_H_

#include <stdint.h>
#include "mliVec.h"
#include "mliOBB.h"

struct mliTriangle {
        struct mliVec v1;
        struct mliVec v2;
        struct mliVec v3;
};

struct mliOBB mliTriangle_obb(
        const struct mliVec a,
        const struct mliVec b,
        const struct mliVec c);
int mliTriangle_has_overlap_obb(
        const struct mliVec a,
        const struct mliVec b,
        const struct mliVec c,
        const struct mliOBB obb);
void _mliTriangle_transform_into_obb(
        const struct mliVec a,
        const struct mliVec b,
        const struct mliVec c,
        const struct mliOBB obb,
        struct mliVec *a_out,
        struct mliVec *b_out,
        struct mliVec *c_out);
int64_t _mli_triangle_cube_intersection(struct mliTriangle t);
int64_t _mli_point_triangle_intersection(
        struct mliVec p,
        struct mliTriangle t);
int64_t _mli_check_line(
        struct mliVec p1,
        struct mliVec p2,
        int64_t outcode_diff);
int64_t _mli_check_point(
        struct mliVec p1,
        struct mliVec p2,
        double alpha,
        int64_t mask);
int64_t _mli_bevel_3d(struct mliVec p);
int64_t _mli_bevel_2d(struct mliVec p);
int64_t _mli_face_plane(struct mliVec p);
#endif
