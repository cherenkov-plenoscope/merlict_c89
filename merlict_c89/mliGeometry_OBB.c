/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliGeometry_OBB.h"
#include "mliHomTra.h"
#include "mliObject.h"
#include "mliObject_OBB.h"

int mliGeometry_robject_has_overlap_obb(
        const struct mliGeometry *geometry,
        const uint32_t robject_idx,
        const struct mliOBB obb)
{
        const uint32_t object_idx = geometry->robjects[robject_idx];
        const struct mliObject *obj_ptr = &geometry->objects[object_idx];
        const struct mliHomTraComp local2root_comp =
                geometry->robject2root[robject_idx];

        return mliObject_has_overlap_obb(obj_ptr, local2root_comp, obb);
}

int mliGeometry_robject_has_overlap_obb_void(
        const void *geometry,
        const uint32_t robject_idx,
        const struct mliOBB obb)
{
        return mliGeometry_robject_has_overlap_obb(
                (const struct mliGeometry *)geometry, robject_idx, obb);
}
