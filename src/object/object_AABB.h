/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_OBJECT_AABB_H_
#define MLI_OBJECT_AABB_H_

#include <stdint.h>
#include "../bool/bool.h"
#include "../homtra/homtra.h"
#include "object.h"
#include "../aabb/aabb.h"

mli_bool mli_Object_has_overlap_aabb(
        const struct mli_Object *obj,
        const struct mli_HomTra local2root,
        const struct mli_AABB aabb);

struct mli_AABB mli_Object_aabb(
        const struct mli_Object *obj,
        const struct mli_HomTra local2root);

mli_bool mli_Object_face_in_local_frame_has_overlap_aabb(
        const struct mli_Object *obj,
        const uint64_t face_idx,
        const struct mli_AABB aabb);

mli_bool mli_Object_face_in_local_frame_has_overlap_aabb_void(
        const void *obj,
        const uint32_t face_idx,
        const struct mli_AABB aabb);

struct mli_AABB mli_Object_aabb_in_local_frame(const struct mli_Object *obj);

#endif
