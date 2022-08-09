/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIGEOMETRY_OBB_H_
#define MLIGEOMETRY_OBB_H_

#include "mliGeometry.h"
#include "mliOBB.h"

int mliGeometry_robject_has_overlap_obb_void(
        const void *geometry,
        const uint32_t robject_idx,
        const struct mliOBB obb);

int mliGeometry_robject_has_overlap_obb(
        const struct mliGeometry *geometry,
        const uint32_t robject_idx,
        const struct mliOBB obb);

#endif
