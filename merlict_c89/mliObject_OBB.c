/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliObject_OBB.h"

int mliObject_face_in_local_frame_has_overlap_obb(
        const struct mliObject *obj,
        const uint64_t face_idx,
        const struct mliOBB obb)
{
        struct mliFace face;
        if (face_idx >= obj->num_faces) {
                return 0;
        }
        face = obj->faces_vertices[face_idx];
        if (mliTriangle_has_overlap_obb(
                obj->vertices[face.a],
                obj->vertices[face.b],
                obj->vertices[face.c],
                obb)) {
                return 1;
        }
        return 0;
}

int mliObject_has_overlap_obb(
        const struct mliObject *obj,
        const struct mliHomTraComp local2root_comp,
        const struct mliOBB obb)
{
        struct mliHomTra local2root = mliHomTra_from_compact(local2root_comp);

        uint64_t face_idx;
        for (face_idx = 0; face_idx < obj->num_faces; face_idx++) {
                const struct mliFace face = obj->faces_vertices[face_idx];

                const struct mliVec a_local = obj->vertices[face.a];
                const struct mliVec b_local = obj->vertices[face.b];
                const struct mliVec c_local = obj->vertices[face.c];

                const struct mliVec a_root = mliHomTra_pos(
                        &local2root,
                        a_local
                );
                const struct mliVec b_root = mliHomTra_pos(
                        &local2root,
                        b_local
                );
                const struct mliVec c_root = mliHomTra_pos(
                        &local2root,
                        c_local
                );

                if (mliTriangle_has_overlap_obb(a_root, b_root, c_root, obb)) {
                        return 1;
                }
        }
        return 0;
}

int _mliObject_face_in_local_frame_has_overlap_obb(
        const void *obj,
        const uint32_t face_idx,
        const struct mliOBB obb)
{
        return mliObject_face_in_local_frame_has_overlap_obb(
                (const struct mliObject *)obj,
                (uint64_t)face_idx,
                obb);
}

struct mliOBB mliObject_obb(
        const struct mliObject *obj,
        const struct mliHomTraComp local2root_comp)
{
        struct mliOBB obb;
        struct mliHomTra local2root = mliHomTra_from_compact(local2root_comp);

        struct mliVec seed_vertex_local;
        struct mliVec seed_vertex_root;
        struct mliFace seed_face;
        uint64_t face_idx;

        if (obj->num_faces == 0) {
                obb.lower = mliVec_set(MLI_NAN, MLI_NAN, MLI_NAN);
                obb.upper = mliVec_set(MLI_NAN, MLI_NAN, MLI_NAN);
                return obb;
        }

        seed_face = obj->faces_vertices[0];
        seed_vertex_local = obj->vertices[seed_face.a];
        seed_vertex_root = mliHomTra_pos(&local2root, seed_vertex_local);

        obb.lower = seed_vertex_root;
        obb.upper = seed_vertex_root;
        for (face_idx = 0; face_idx < obj->num_faces; face_idx++) {
                const struct mliFace face = obj->faces_vertices[face_idx];
                const struct mliVec a_local = obj->vertices[face.a];
                const struct mliVec b_local = obj->vertices[face.b];
                const struct mliVec c_local = obj->vertices[face.c];
                const struct mliVec a_root = mliHomTra_pos(
                        &local2root,
                        a_local
                );
                const struct mliVec b_root = mliHomTra_pos(
                        &local2root,
                        b_local
                );
                const struct mliVec c_root = mliHomTra_pos(
                        &local2root,
                        c_local
                );

                obb.lower.x = MLI_MIN2(obb.lower.x, a_root.x);
                obb.lower.y = MLI_MIN2(obb.lower.y, a_root.y);
                obb.lower.z = MLI_MIN2(obb.lower.z, a_root.z);

                obb.upper.x = MLI_MAX2(obb.upper.x, a_root.x);
                obb.upper.y = MLI_MAX2(obb.upper.y, a_root.y);
                obb.upper.z = MLI_MAX2(obb.upper.z, a_root.z);

                obb.lower.x = MLI_MIN2(obb.lower.x, b_root.x);
                obb.lower.y = MLI_MIN2(obb.lower.y, b_root.y);
                obb.lower.z = MLI_MIN2(obb.lower.z, b_root.z);

                obb.upper.x = MLI_MAX2(obb.upper.x, b_root.x);
                obb.upper.y = MLI_MAX2(obb.upper.y, b_root.y);
                obb.upper.z = MLI_MAX2(obb.upper.z, b_root.z);

                obb.lower.x = MLI_MIN2(obb.lower.x, c_root.x);
                obb.lower.y = MLI_MIN2(obb.lower.y, c_root.y);
                obb.lower.z = MLI_MIN2(obb.lower.z, c_root.z);

                obb.upper.x = MLI_MAX2(obb.upper.x, c_root.x);
                obb.upper.y = MLI_MAX2(obb.upper.y, c_root.y);
                obb.upper.z = MLI_MAX2(obb.upper.z, c_root.z);
        }
        return obb;
}

struct mliOBB mliObject_obb_in_local_frame(const struct mliObject *obj)
{
        const struct mliHomTraComp unity_comp = mliHomTraComp_set(
                mliVec_set(0.0, 0.0, 0.0),
                mliQuaternion_set(0.0, 0.0, 0.0, 1.0));
        return mliObject_obb(obj, unity_comp);
}
