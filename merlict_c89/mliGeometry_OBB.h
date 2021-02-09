/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIGEOMETRY_OBB_H_
#define MERLICT_C89_MLIGEOMETRY_OBB_H_

#include "mliGeometry.h"
#include "mliOBB.h"

int _mliGeometry_robject_has_overlap_obb(
        const void *scenery,
        const uint32_t robject_idx,
        const struct mliOBB obb);

int mliGeometry_robject_has_overlap_obb(
        const struct mliGeometry *scenery,
        const uint32_t robject_idx,
        const struct mliOBB obb);

#endif
