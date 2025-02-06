/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "object_AABB.h"
#include "../triangle/triangle_aabb.h"
#include "../math/math.h"

mli_bool mli_Object_face_in_local_frame_has_overlap_aabb(
        const struct mli_Object *obj,
        const uint64_t face_idx,
        const struct mli_AABB aabb)
{
        struct mli_object_Face face;
        if (face_idx >= obj->num_faces) {
                return MLI_FALSE;
        }
        face = obj->faces_vertices[face_idx];
        if (mli_Triangle_has_overlap_aabb(
                    obj->vertices[face.a],
                    obj->vertices[face.b],
                    obj->vertices[face.c],
                    aabb)) {
                return MLI_TRUE;
        }
        return MLI_FALSE;
}

mli_bool mli_Object_has_overlap_aabb(
        const struct mli_Object *obj,
        const struct mli_HomTra local2root,
        const struct mli_AABB aabb)
{
        uint64_t face_idx;
        for (face_idx = 0; face_idx < obj->num_faces; face_idx++) {
                const struct mli_object_Face face =
                        obj->faces_vertices[face_idx];

                const struct mli_Vec a_local = obj->vertices[face.a];
                const struct mli_Vec b_local = obj->vertices[face.b];
                const struct mli_Vec c_local = obj->vertices[face.c];

                const struct mli_Vec a_root =
                        mli_HomTraComp_pos(&local2root, a_local);
                const struct mli_Vec b_root =
                        mli_HomTraComp_pos(&local2root, b_local);
                const struct mli_Vec c_root =
                        mli_HomTraComp_pos(&local2root, c_local);

                if (mli_Triangle_has_overlap_aabb(
                            a_root, b_root, c_root, aabb)) {
                        return MLI_TRUE;
                }
        }
        return MLI_FALSE;
}

mli_bool mli_Object_face_in_local_frame_has_overlap_aabb_void(
        const void *obj,
        const uint32_t face_idx,
        const struct mli_AABB aabb)
{
        return mli_Object_face_in_local_frame_has_overlap_aabb(
                (const struct mli_Object *)obj, (uint64_t)face_idx, aabb);
}

struct mli_AABB mli_Object_aabb(
        const struct mli_Object *obj,
        const struct mli_HomTra local2root)
{
        struct mli_AABB aabb;
        struct mli_Vec seed_vertex_local;
        struct mli_Vec seed_vertex_root;
        struct mli_object_Face seed_face;
        uint64_t face_idx;

        if (obj->num_faces == 0) {
                aabb.lower =
                        mli_Vec_init(MLI_MATH_NAN, MLI_MATH_NAN, MLI_MATH_NAN);
                aabb.upper =
                        mli_Vec_init(MLI_MATH_NAN, MLI_MATH_NAN, MLI_MATH_NAN);
                return aabb;
        }

        seed_face = obj->faces_vertices[0];
        seed_vertex_local = obj->vertices[seed_face.a];
        seed_vertex_root = mli_HomTraComp_pos(&local2root, seed_vertex_local);

        aabb.lower = seed_vertex_root;
        aabb.upper = seed_vertex_root;
        for (face_idx = 0; face_idx < obj->num_faces; face_idx++) {
                const struct mli_object_Face face =
                        obj->faces_vertices[face_idx];
                const struct mli_Vec a_local = obj->vertices[face.a];
                const struct mli_Vec b_local = obj->vertices[face.b];
                const struct mli_Vec c_local = obj->vertices[face.c];
                const struct mli_Vec a_root =
                        mli_HomTraComp_pos(&local2root, a_local);
                const struct mli_Vec b_root =
                        mli_HomTraComp_pos(&local2root, b_local);
                const struct mli_Vec c_root =
                        mli_HomTraComp_pos(&local2root, c_local);

                aabb.lower.x = MLI_MATH_MIN2(aabb.lower.x, a_root.x);
                aabb.lower.y = MLI_MATH_MIN2(aabb.lower.y, a_root.y);
                aabb.lower.z = MLI_MATH_MIN2(aabb.lower.z, a_root.z);

                aabb.upper.x = MLI_MATH_MAX2(aabb.upper.x, a_root.x);
                aabb.upper.y = MLI_MATH_MAX2(aabb.upper.y, a_root.y);
                aabb.upper.z = MLI_MATH_MAX2(aabb.upper.z, a_root.z);

                aabb.lower.x = MLI_MATH_MIN2(aabb.lower.x, b_root.x);
                aabb.lower.y = MLI_MATH_MIN2(aabb.lower.y, b_root.y);
                aabb.lower.z = MLI_MATH_MIN2(aabb.lower.z, b_root.z);

                aabb.upper.x = MLI_MATH_MAX2(aabb.upper.x, b_root.x);
                aabb.upper.y = MLI_MATH_MAX2(aabb.upper.y, b_root.y);
                aabb.upper.z = MLI_MATH_MAX2(aabb.upper.z, b_root.z);

                aabb.lower.x = MLI_MATH_MIN2(aabb.lower.x, c_root.x);
                aabb.lower.y = MLI_MATH_MIN2(aabb.lower.y, c_root.y);
                aabb.lower.z = MLI_MATH_MIN2(aabb.lower.z, c_root.z);

                aabb.upper.x = MLI_MATH_MAX2(aabb.upper.x, c_root.x);
                aabb.upper.y = MLI_MATH_MAX2(aabb.upper.y, c_root.y);
                aabb.upper.z = MLI_MATH_MAX2(aabb.upper.z, c_root.z);
        }
        return aabb;
}

struct mli_AABB mli_Object_aabb_in_local_frame(const struct mli_Object *obj)
{
        struct mli_HomTra unity;
        unity.translation = mli_Vec_init(0.0, 0.0, 0.0);
        unity.rotation = mli_Mat_unity();
        return mli_Object_aabb(obj, unity);
}
