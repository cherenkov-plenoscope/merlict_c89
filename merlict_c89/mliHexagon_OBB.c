/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliHexagon_OBB.h"

int mliHexagon_has_overlap_obb(
        const struct mliHexagon hex,
        const struct mliHomTraComp local2root_comp,
        const struct mliOBB obb)
{
        struct mliHomTra local2root = mliHomTra_from_compact(local2root_comp);
        /*
         *  Either one of the hexagons corners is inside the cube, or one of the
         *  cube's edges has to intersect the hexagon.
         */
        uint64_t corner_idx;
        uint64_t edge_idx;
        for (corner_idx = 0; corner_idx < 5; corner_idx++) {
                const struct mliVec corner_local =
                        mli_hexagon_corner(corner_idx);
                const struct mliVec corner_root =
                        mliHomTra_pos(&local2root, corner_local);
                if (mliVec_overlap_obb(corner_root, obb.lower, obb.upper)) {
                        return 1;
                }
        }
        for (edge_idx = 0; edge_idx < 12; edge_idx++) {
                struct mliEdge edge = mliOBB_edge(obb, edge_idx);
                struct mliIntersection intersection;
                if (mliHexagon_intersection(
                            hex, local2root_comp, edge.ray, &intersection)) {
                        if (intersection.distance_of_ray <= edge.length) {
                                return 1;
                        }
                }
        }
        return 0;
}

struct mliOBB mliHexagon_obb(
        const struct mliHexagon hex,
        const struct mliHomTraComp local2root_comp)
{
        struct mliOBB obb;
        struct mliHomTra local2root = mliHomTra_from_compact(local2root_comp);
        struct mliVec first_corner_local = mliVec_multiply(
                mli_hexagon_corner(0), hex.inner_radius * MLI_2_OVER_SQRT3);
        struct mliVec first_corner_root =
                mliHomTra_pos(&local2root, first_corner_local);
        uint64_t corner_idx;
        obb.lower = first_corner_root;
        obb.upper = first_corner_root;
        for (corner_idx = 1; corner_idx < 5; corner_idx++) {
                struct mliVec corner_root;
                struct mliVec corner_local = mliVec_multiply(
                        mli_hexagon_corner(corner_idx),
                        hex.inner_radius * MLI_2_OVER_SQRT3);
                corner_root = mliHomTra_pos(&local2root, corner_local);
                obb.lower.x = MLI_MIN2(obb.lower.x, corner_root.x);
                obb.lower.y = MLI_MIN2(obb.lower.y, corner_root.y);
                obb.lower.z = MLI_MIN2(obb.lower.z, corner_root.z);

                obb.upper.x = MLI_MAX2(obb.upper.x, corner_root.x);
                obb.upper.y = MLI_MAX2(obb.upper.y, corner_root.y);
                obb.upper.z = MLI_MAX2(obb.upper.z, corner_root.z);
        }
        return obb;
}