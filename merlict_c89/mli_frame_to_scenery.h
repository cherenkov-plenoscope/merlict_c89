/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_FRAME_TO_SCENERY_H_
#define MERLICT_C89_MLI_FRAME_TO_SCENERY_H_

#include <stdint.h>
#include "mliGeometry.h"
#include "mliUserScenery.h"

int __mliGeometry_set_robjects(
        struct mliGeometry *scenery,
        const struct mliFrame *frame,
        uint64_t *robject_counter);
int __mliGeometry_estimate_num_robjects_in_frames(
        uint64_t *num_robjects,
        const struct mliFrame *frame);
#endif
