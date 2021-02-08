/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliCombine.h"
#include "mliHomTra.h"
#include "mliObject.h"
#include "mliObject_OBB.h"

int mliCombine_robject_has_overlap_obb(
        const struct mliScenery *scenery,
        const uint32_t robject_idx,
        const struct mliOBB obb)
{
        const uint32_t object_idx = scenery->robjects[robject_idx];
        const struct mliObject *obj_ptr =
                &scenery->resources.objects[object_idx];
        const struct mliHomTraComp local2root_comp =
                scenery->robject2root[robject_idx];

        return mliObject_has_overlap_obb(obj_ptr, local2root_comp, obb);
}

int _mliCombine_robject_has_overlap_obb(
        const void *scenery,
        const uint32_t robject_idx,
        const struct mliOBB obb)
{
        return mliCombine_robject_has_overlap_obb(
                (const struct mliScenery *)scenery, robject_idx, obb);
}
