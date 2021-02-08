/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLISCENERY_OBB_H_
#define MERLICT_C89_MLISCENERY_OBB_H_

#include "mliScenery.h"
#include "mliOBB.h"

int _mliScenery_robject_has_overlap_obb(
        const void *scenery,
        const uint32_t robject_idx,
        const struct mliOBB obb);

int mliScenery_robject_has_overlap_obb(
        const struct mliScenery *scenery,
        const uint32_t robject_idx,
        const struct mliOBB obb);

#endif
