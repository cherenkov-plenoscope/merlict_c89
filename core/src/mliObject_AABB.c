/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliObject_AABB.h"
#include "mliTriangle_AABB.h"

int mliObject_face_in_local_frame_has_overlap_aabb(
        const struct mliObject *obj,
        const uint64_t face_idx,
        const struct mliAABB aabb)
{
        struct mliFace face;
        if (face_idx >= obj->num_faces) {
                return 0;
        }
        face = obj->faces_vertices[face_idx];
        if (mliTriangle_has_overlap_aabb(
                    obj->vertices[face.a],
                    obj->vertices[face.b],
                    obj->vertices[face.c],
                    aabb)) {
                return 1;
        }
        return 0;
}

int mliObject_has_overlap_aabb(
        const struct mliObject *obj,
        const struct mliHomTra local2root,
        const struct mliAABB aabb)
{
        uint64_t face_idx;
        for (face_idx = 0; face_idx < obj->num_faces; face_idx++) {
                const struct mliFace face = obj->faces_vertices[face_idx];

                const struct mliVec a_local = obj->vertices[face.a];
                const struct mliVec b_local = obj->vertices[face.b];
                const struct mliVec c_local = obj->vertices[face.c];

                const struct mliVec a_root =
                        mliHomTra_pos(&local2root, a_local);
                const struct mliVec b_root =
                        mliHomTra_pos(&local2root, b_local);
                const struct mliVec c_root =
                        mliHomTra_pos(&local2root, c_local);

                if (mliTriangle_has_overlap_aabb(
                            a_root, b_root, c_root, aabb)) {
                        return 1;
                }
        }
        return 0;
}

int mliObject_face_in_local_frame_has_overlap_aabb_void(
        const void *obj,
        const uint32_t face_idx,
        const struct mliAABB aabb)
{
        return mliObject_face_in_local_frame_has_overlap_aabb(
                (const struct mliObject *)obj, (uint64_t)face_idx, aabb);
}

struct mliAABB mliObject_aabb(
        const struct mliObject *obj,
        const struct mliHomTra local2root)
{
        struct mliAABB aabb;
        struct mliVec seed_vertex_local;
        struct mliVec seed_vertex_root;
        struct mliFace seed_face;
        uint64_t face_idx;

        if (obj->num_faces == 0) {
                aabb.lower = mliVec_init(MLI_NAN, MLI_NAN, MLI_NAN);
                aabb.upper = mliVec_init(MLI_NAN, MLI_NAN, MLI_NAN);
                return aabb;
        }

        seed_face = obj->faces_vertices[0];
        seed_vertex_local = obj->vertices[seed_face.a];
        seed_vertex_root = mliHomTra_pos(&local2root, seed_vertex_local);

        aabb.lower = seed_vertex_root;
        aabb.upper = seed_vertex_root;
        for (face_idx = 0; face_idx < obj->num_faces; face_idx++) {
                const struct mliFace face = obj->faces_vertices[face_idx];
                const struct mliVec a_local = obj->vertices[face.a];
                const struct mliVec b_local = obj->vertices[face.b];
                const struct mliVec c_local = obj->vertices[face.c];
                const struct mliVec a_root =
                        mliHomTra_pos(&local2root, a_local);
                const struct mliVec b_root =
                        mliHomTra_pos(&local2root, b_local);
                const struct mliVec c_root =
                        mliHomTra_pos(&local2root, c_local);

                aabb.lower.x = MLI_MIN2(aabb.lower.x, a_root.x);
                aabb.lower.y = MLI_MIN2(aabb.lower.y, a_root.y);
                aabb.lower.z = MLI_MIN2(aabb.lower.z, a_root.z);

                aabb.upper.x = MLI_MAX2(aabb.upper.x, a_root.x);
                aabb.upper.y = MLI_MAX2(aabb.upper.y, a_root.y);
                aabb.upper.z = MLI_MAX2(aabb.upper.z, a_root.z);

                aabb.lower.x = MLI_MIN2(aabb.lower.x, b_root.x);
                aabb.lower.y = MLI_MIN2(aabb.lower.y, b_root.y);
                aabb.lower.z = MLI_MIN2(aabb.lower.z, b_root.z);

                aabb.upper.x = MLI_MAX2(aabb.upper.x, b_root.x);
                aabb.upper.y = MLI_MAX2(aabb.upper.y, b_root.y);
                aabb.upper.z = MLI_MAX2(aabb.upper.z, b_root.z);

                aabb.lower.x = MLI_MIN2(aabb.lower.x, c_root.x);
                aabb.lower.y = MLI_MIN2(aabb.lower.y, c_root.y);
                aabb.lower.z = MLI_MIN2(aabb.lower.z, c_root.z);

                aabb.upper.x = MLI_MAX2(aabb.upper.x, c_root.x);
                aabb.upper.y = MLI_MAX2(aabb.upper.y, c_root.y);
                aabb.upper.z = MLI_MAX2(aabb.upper.z, c_root.z);
        }
        return aabb;
}

struct mliAABB mliObject_aabb_in_local_frame(const struct mliObject *obj)
{
        struct mliHomTra unity;
        unity.translation = mliVec_init(0.0, 0.0, 0.0);
        unity.rotation = mliMat_unity();
        return mliObject_aabb(obj, unity);
}
