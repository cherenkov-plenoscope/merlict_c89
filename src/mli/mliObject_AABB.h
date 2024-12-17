/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIOBJECT_AABB_H_
#define MLIOBJECT_AABB_H_

#include <stdint.h>
#include "../homtra/homtra.h"
#include "mliObject.h"
#include "../aabb/aabb.h"

int mliObject_has_overlap_aabb(
        const struct mliObject *obj,
        const struct mli_HomTra local2root,
        const struct mli_AABB aabb);

struct mli_AABB mliObject_aabb(
        const struct mliObject *obj,
        const struct mli_HomTra local2root);

int mliObject_face_in_local_frame_has_overlap_aabb(
        const struct mliObject *obj,
        const uint64_t face_idx,
        const struct mli_AABB aabb);

int mliObject_face_in_local_frame_has_overlap_aabb_void(
        const void *obj,
        const uint32_t face_idx,
        const struct mli_AABB aabb);

struct mli_AABB mliObject_aabb_in_local_frame(const struct mliObject *obj);

#endif
