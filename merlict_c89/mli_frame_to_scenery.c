/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_frame_to_scenery.h"
#include <math.h>
#include "mli_debug.h"

int __mliScenery_estimate_num_robjects_in_frames(
        uint64_t *num_robjects,
        const struct mliFrame *frame)
{
        uint64_t c;
        switch (frame->type) {
        case MLI_FRAME:
                for (c = 0; c < frame->children.dyn.size; c++) {
                        struct mliFrame *child = frame->children.arr[c];
                        mli_c(__mliScenery_estimate_num_robjects_in_frames(
                                num_robjects, child));
                }
                break;
        case MLI_OBJECT:
                (*num_robjects) += 1;
                break;
        default:
                mli_sentinel("Expected either type 'frame' or 'object'.");
                break;
        }
        return 1;
error:
        return 0;
}

int __mliScenery_set_robjects(
        struct mliScenery *scenery,
        const struct mliFrame *frame,
        uint64_t *robject_counter)
{
        uint64_t c;
        uint64_t rob;
        struct mliFrame *child;
        switch (frame->type) {
        case MLI_FRAME:
                for (c = 0; c < frame->children.dyn.size; c++) {
                        child = frame->children.arr[c];
                        __mliScenery_set_robjects(
                                scenery, child, robject_counter);
                }
                break;
        case MLI_OBJECT:
                rob = (*robject_counter);
                scenery->robjects[rob] = frame->object;
                scenery->robject_boundary_layers[rob] = frame->boundary_layer;
                scenery->robject2root[rob] = frame->frame2root;
                scenery->robject_ids[rob] = frame->id;
                (*robject_counter) += 1;
                break;
        default:
                mli_sentinel("Expected either type 'frame' or 'object'.");
                break;
        }
        return 1;
error:
        return 0;
}
