/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIDBICIRCLEPLANE_OBB_H_
#define MERLICT_MLIDBICIRCLEPLANE_OBB_H_

#include <math.h>
#include <assert.h>
#include "mliOBB.h"
#include "mliBiCirclePlane.h"
#include "mliTriangle_OBB.h"


struct mliVec __mliBiCirclePlane_rectangle_corners(
    const struct mliBiCirclePlane plane,
    const uint64_t corner_idx) {
    /*
     *  c1__height__c0
     *   |         /|
     *   |        / |
     *   |       /  |    |y
     *   |      /   |    |
     *   |     /    |    |_____ x
     *   |    /     |
     *   |   /      |-width
     *   |  /       |
     *   | /        |
     *   |/_________|
     *  c2          c3
     */
    const double hxh = plane.x_height*.5;
    const double hyw = plane.y_width*.5;
    switch (corner_idx) {
        case 0:
            return mliVec_set(hxh, hyw, 0.);
            break;
        case 1:
            return mliVec_set(-hxh, hyw, 0.);
            break;
        case 2:
            return mliVec_set(-hxh, -hyw, 0.);
            break;
        case 3:
            return mliVec_set(hxh, -hyw, 0.);
            break;
        default:
            assert(0);
    }
}

int mliBiCirclePlane_has_overlap_obb(
    const struct mliBiCirclePlane plane,
    const struct mliHomTraComp local2root_comp,
    const struct mliOBB obb) {
    struct mliHomTra local2root = mliHomTra_from_compact(local2root_comp);
    struct mliVec c0_local, c1_local, c2_local, c3_local,
             c0_root, c1_root, c2_root, c3_root;
    /* approximation using two triangles */
    c0_local = __mliBiCirclePlane_rectangle_corners(plane, 0);
    c1_local = __mliBiCirclePlane_rectangle_corners(plane, 1);
    c2_local = __mliBiCirclePlane_rectangle_corners(plane, 2);
    c3_local = __mliBiCirclePlane_rectangle_corners(plane, 3);

    c0_root = mliHomTra_pos(&local2root, c0_local);
    c1_root = mliHomTra_pos(&local2root, c1_local);
    c2_root = mliHomTra_pos(&local2root, c2_local);
    c3_root = mliHomTra_pos(&local2root, c3_local);

    return mliTriangle_has_overlap_obb(c0_root, c1_root, c2_root, obb) ||
            mliTriangle_has_overlap_obb(c2_root, c3_root, c0_root, obb);
}

struct mliOBB mliBiCirclePlane_obb(
    const struct mliBiCirclePlane plane,
    const struct mliHomTraComp local2root_comp) {
    struct mliOBB obb;
    uint64_t corner_idx;
    struct mliVec c0_local, c0_root;
    struct mliHomTra local2root = mliHomTra_from_compact(local2root_comp);

    c0_local = __mliBiCirclePlane_rectangle_corners(plane, 0);
    c0_root = mliHomTra_pos(&local2root, c0_local);

    obb.lower = c0_root;
    obb.upper = c0_root;
    for (corner_idx = 1; corner_idx < 4; corner_idx ++) {
        struct mliVec c_local, c_root;
        c_local = __mliBiCirclePlane_rectangle_corners(plane, corner_idx);
        c_root = mliHomTra_pos(&local2root, c_local);
        obb.lower.x = MLI_MIN2(obb.lower.x, c_root.x);
        obb.lower.y = MLI_MIN2(obb.lower.y, c_root.y);
        obb.lower.z = MLI_MIN2(obb.lower.z, c_root.z);

        obb.upper.x = MLI_MAX2(obb.upper.x, c_root.x);
        obb.upper.y = MLI_MAX2(obb.upper.y, c_root.y);
        obb.upper.z = MLI_MAX2(obb.upper.z, c_root.z);
    }

    return obb;
}

#endif
