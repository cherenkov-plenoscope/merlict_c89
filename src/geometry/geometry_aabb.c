/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "geometry_aabb.h"
#include "geometry_and_accelerator.h"
#include "../homtra/homtra.h"
#include "../object/object.h"
#include "../object/object_AABB.h"
#include "../aabb/aabb.h"

mli_bool mli_Geometry_robject_has_overlap_aabb(
        const struct mli_GeometryAndAccelerator *accgeo,
        const uint32_t robject_idx,
        const struct mli_AABB aabb)
{
        const struct mli_AABB robject_aabb =
                accgeo->accelerator->robject_aabbs[robject_idx];
        if (mli_AABB_is_overlapping(aabb, robject_aabb)) {
                /* Only test the object's faces when its own aabb has an
                 * overlap with the aabb to test for.
                 */
                const uint32_t obj_idx =
                        accgeo->geometry->robjects[robject_idx];
                const struct mli_Object *obj_ptr =
                        &accgeo->geometry->objects[obj_idx];
                const struct mli_HomTra robj2root = mli_HomTraComp_from_compact(
                        accgeo->geometry->robject2root[robject_idx]);
                return mli_Object_has_overlap_aabb(obj_ptr, robj2root, aabb);
        } else {
                return MLI_FALSE;
        }
}

mli_bool mli_Geometry_robject_has_overlap_aabb_void(
        const void *accgeo,
        const uint32_t robject_idx,
        const struct mli_AABB aabb)
{
        return mli_Geometry_robject_has_overlap_aabb(
                (const struct mli_GeometryAndAccelerator *)accgeo,
                robject_idx,
                aabb);
}
