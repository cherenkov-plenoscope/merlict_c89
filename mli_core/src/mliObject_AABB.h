/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIOBJECT_AABB_H_
#define MLIOBJECT_AABB_H_

#include <stdint.h>
#include "mliHomTra.h"
#include "mliObject.h"
#include "mliAABB.h"

int mliObject_has_overlap_aabb(
        const struct mliObject *obj,
        const struct mliHomTra local2root,
        const struct mliAABB aabb);

struct mliAABB mliObject_aabb(
        const struct mliObject *obj,
        const struct mliHomTra local2root);

int mliObject_face_in_local_frame_has_overlap_aabb(
        const struct mliObject *obj,
        const uint64_t face_idx,
        const struct mliAABB aabb);

int mliObject_face_in_local_frame_has_overlap_aabb_void(
        const void *obj,
        const uint32_t face_idx,
        const struct mliAABB aabb);

struct mliAABB mliObject_aabb_in_local_frame(const struct mliObject *obj);

#endif
