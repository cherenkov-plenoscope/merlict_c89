/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_frame_to_scenery.h"

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
                mli_sentinel("Expected either type 'Frame' or 'Object'.");
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
        uint64_t i;
        struct mliFrame *child;
        switch (frame->type) {
        case MLI_FRAME:
                for (i = 0; i < frame->children.dyn.size; i++) {
                        child = frame->children.arr[i];
                        __mliScenery_set_robjects(scenery, child, robject_counter);
                }
                break;
        case MLI_OBJECT:
                i = (*robject_counter);
                scenery->robject_boundary_layers[i] = frame->boundary_layer;
                scenery->robject2root[i] = frame->frame2root;
                scenery->robject_user_ids[i] = frame->id;
                (*robject_counter) += 1;
                break;
        default:
                mli_sentinel("Unknown type of frame.");
                break;
        }
        return 1;
error:
        return 0;
}

int mliScenery_malloc_from_mliUserScenery(
        struct mliScenery *scenery,
        const struct mliUserScenery *uscn)
{
        uint64_t num_robjects = 0u;
        uint64_t robject_counter = 0u;
        struct mliSceneryResourcesCapacity resource_capacity =
                mliSceneryResourcesCapacity_init();

        /* resource_capacity */
        resource_capacity.num_objects = uscn->resources.num_objects;
        resource_capacity.num_functions = uscn->resources.num_functions;
        resource_capacity.num_colors = uscn->resources.num_colors;
        resource_capacity.num_media = uscn->resources.num_media;
        resource_capacity.num_surfaces = uscn->resources.num_surfaces;

        /* capacity of primitives */
        mli_check(
                __mliScenery_estimate_num_robjects_in_frames(
                        &num_robjects, &uscn->root),
                "Can not estimate capacity of primitives.");

        /* malloc scenery */
        mli_check(mliScenery_malloc(scenery, num_robjects),
                "Can not allocate scenery.");

        mli_check(mliSceneryResources_malloc(
                        &scenery->resources, resource_capacity),
                "Can not allocate scenery.");

        mli_check(
                mliSceneryResources_cpy(&scenery->resources, &uscn->resources),
                "Can not copy mliSceneryResources.");

        mli_check(
                __mliScenery_set_robjects(
                        scenery,
                        &uscn->root,
                        &robject_counter),
                "Cen not set primitives");

        mli_c(num_robjects == robject_counter);

        return 1;
error:
        return 0;
}
