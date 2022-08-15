/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliGeometry_AABB.h"
#include "mliHomTra.h"
#include "mliObject.h"
#include "mliObject_AABB.h"

int mliGeometry_robject_has_overlap_aabb(
        const struct mliGeometry *geometry,
        const uint32_t robject_idx,
        const struct mliAABB aabb)
{
        const uint32_t object_idx = geometry->robjects[robject_idx];
        const struct mliObject *obj_ptr = &geometry->objects[object_idx];
        const struct mliHomTra local2root = mliHomTra_from_compact(
                geometry->robject2root[robject_idx]);

        return mliObject_has_overlap_aabb(obj_ptr, local2root, aabb);
}

int mliGeometry_robject_has_overlap_aabb_void(
        const void *geometry,
        const uint32_t robject_idx,
        const struct mliAABB aabb)
{
        return mliGeometry_robject_has_overlap_aabb(
                (const struct mliGeometry *)geometry, robject_idx, aabb);
}
