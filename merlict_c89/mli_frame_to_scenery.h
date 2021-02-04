/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_FRAME_TO_SCENERY_H_
#define MERLICT_C89_MLI_FRAME_TO_SCENERY_H_

#include <stdint.h>
#include "mliScenery.h"
#include "mliUserScenery.h"

int mliScenery_malloc_from_mliUserScenery(
        struct mliScenery *scenery,
        const struct mliUserScenery *uscn);
int __mliScenery_set_robjects(
        struct mliScenery *scenery,
        const struct mliFrame *frame,
        uint64_t *robject_counter);
int __mliScenery_estimate_num_robjects_in_frames(
        uint64_t *num_robjects,
        const struct mliFrame *frame);
#endif
