/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIOBJECT_OBB_H_
#define MLIOBJECT_OBB_H_

#include "mliHomTra.h"
#include "mliObject.h"
#include "mliOBB.h"

int mliObject_has_overlap_obb(
        const struct mliObject *obj,
        const struct mliHomTraComp local2root_comp,
        const struct mliOBB obb);

struct mliOBB mliObject_obb(
        const struct mliObject *obj,
        const struct mliHomTraComp local2root_comp);

int mliObject_face_in_local_frame_has_overlap_obb(
        const struct mliObject *obj,
        const uint64_t face_idx,
        const struct mliOBB obb);

int mliObject_face_in_local_frame_has_overlap_obb_void(
        const void *obj,
        const uint32_t face_idx,
        const struct mliOBB obb);

struct mliOBB mliObject_obb_lin_local_frame(const struct mliObject *obj);

#endif