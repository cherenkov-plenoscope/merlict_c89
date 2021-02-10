/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLI_FRAME_TO_SCENERY_H_
#define MERLICT_C89_MLI_FRAME_TO_SCENERY_H_

#include <stdint.h>
#include "mliGeometry.h"

int __mliGeometry_set_robjects(
        struct mliGeometry *geometry,
        const struct mliFrame *frame,
        uint64_t *robject_counter);

int mliFrame_estimate_num_robjects_and_total_num_boundary_layers(
        const struct mliFrame *frame,
        uint64_t *num_robjects,
        uint64_t *total_num_boundary_layers);
#endif
