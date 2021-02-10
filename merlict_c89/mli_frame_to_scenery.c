/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_frame_to_scenery.h"
#include <math.h>
#include "mli_debug.h"

int mliFrame_estimate_num_robjects_and_total_num_boundary_layers(
        const struct mliFrame *frame,
        uint64_t *num_robjects,
        uint64_t *total_num_boundary_layers)
{
        uint64_t c;
        switch (frame->type) {
        case MLI_FRAME:
                for (c = 0; c < frame->children.dyn.size; c++) {
                        mli_c(mliFrame_estimate_num_robjects_and_total_num_boundary_layers(
                                frame->children.arr[c],
                                num_robjects,
                                total_num_boundary_layers));
                }
                break;
        case MLI_OBJECT:
                (*num_robjects) += 1;
                (*total_num_boundary_layers) += frame->boundary_layers.dyn.size;
                break;
        default:
                mli_sentinel("Expected either type 'frame' or 'object'.");
                break;
        }
        return 1;
error:
        return 0;
}

int __mliGeometry_set_robjects(
        struct mliGeometry *geometry,
        const struct mliFrame *frame,
        uint64_t *robject_counter)
{
        uint64_t c;
        uint64_t rob;
        switch (frame->type) {
        case MLI_FRAME:
                for (c = 0; c < frame->children.dyn.size; c++) {
                        mli_c(__mliGeometry_set_robjects(
                                geometry,
                                frame->children.arr[c],
                                robject_counter));
                }
                break;
        case MLI_OBJECT:
                rob = (*robject_counter);
                geometry->robjects[rob] = frame->object;
                geometry->robject_boundary_layers[rob] = frame->boundary_layer;
                geometry->robject2root[rob] = frame->frame2root;
                geometry->robject_ids[rob] = frame->id;
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
