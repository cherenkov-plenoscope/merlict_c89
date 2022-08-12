/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIGEOMETRY_AABB_H_
#define MLIGEOMETRY_AABB_H_

#include "mliGeometry.h"
#include "mliAABB.h"

int mliGeometry_robject_has_overlap_aabb_void(
        const void *geometry,
        const uint32_t robject_idx,
        const struct mliAABB aabb);

int mliGeometry_robject_has_overlap_aabb(
        const struct mliGeometry *geometry,
        const uint32_t robject_idx,
        const struct mliAABB aabb);

#endif
