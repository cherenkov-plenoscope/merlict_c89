/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLICOMBINE_H_
#define MERLICT_C89_MLICOMBINE_H_

#include <math.h>
#include <stdint.h>

#include "mli_debug.h"
#include "mliScenery.h"
#include "mliAccelerator.h"

struct mliCombine {
        const struct mliScenery *scenery;
        struct mliAccelerator *accelerator;
};

int _mliCombine_robject_has_overlap_obb(
        const void *scenery,
        const uint32_t robject_idx,
        const struct mliOBB obb);

int mliCombine_robject_has_overlap_obb(
        const struct mliScenery *scenery,
        const uint32_t robject_idx,
        const struct mliOBB obb);

#endif
