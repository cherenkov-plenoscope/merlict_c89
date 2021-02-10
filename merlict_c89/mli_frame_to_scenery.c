/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_frame_to_scenery.h"
#include <math.h>
#include "mli_debug.h"

int __mliFrame_estimate_num_robjects_and_total_num_boundary_layers(
        const struct mliFrame *frame,
        uint64_t *num_robjects,
        uint64_t *total_num_boundary_layers)
{
        uint64_t c;
        switch (frame->type) {
        case MLI_FRAME:
                for (c = 0; c < frame->children.dyn.size; c++) {
                        mli_c(__mliFrame_estimate_num_robjects_and_total_num_boundary_layers(
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

int mliFrame_estimate_num_robjects_and_total_num_boundary_layers(
        const struct mliFrame *frame,
        uint64_t *num_robjects,
        uint64_t *total_num_boundary_layers)
{
        (*num_robjects) = 0u;
        (*total_num_boundary_layers) = 0u;
        mli_check(
                __mliFrame_estimate_num_robjects_and_total_num_boundary_layers(
                        frame,
                        num_robjects,
                        total_num_boundary_layers),
                "Failed to walk tree of frames to estimate "
                "num_robjects and total_num_boundary_layers.");
        return 1;
error:
        return 0;
}

int __mliFrame_set_robjects_and_material_map(
        const struct mliFrame *frame,
        struct mliGeometry *geometry,
        struct mliGeometryToMaterialMap *geomap,
        uint64_t *num_robjects,
        uint64_t *total_num_boundary_layers)
{
        uint64_t c;
        uint64_t material_idx;
        uint64_t robject_idx;
        switch (frame->type) {
        case MLI_FRAME:
                for (c = 0; c < frame->children.dyn.size; c++) {
                        mli_c(__mliFrame_set_robjects_and_material_map(
                                frame->children.arr[c],
                                geometry,
                                geomap,
                                num_robjects,
                                total_num_boundary_layers));
                }
                break;
        case MLI_OBJECT:
                robject_idx = (*num_robjects);

                mli_check(
                        frame->object < geometry->num_objects,
                        "Expected frame->object < num_objects.");
                /* geometry */
                geometry->robjects[robject_idx] = frame->object;
                geometry->robject2root[robject_idx] = frame->frame2root;
                geometry->robject_ids[robject_idx] = frame->id;

                /* materials map */
                mli_check(
                        frame->boundary_layers.dyn.size ==
                        geometry->objects[frame->object].num_materials,
                        "Expected Frame to have same "
                        "num boundary_layers as object.");

                geomap->first_boundary_layer_in_robject[robject_idx] =
                        (*total_num_boundary_layers);
                for (
                        material_idx = 0;
                        material_idx < frame->boundary_layers.dyn.size;
                        material_idx++)
                {
                        mliGeometryToMaterialMap_set(
                                geomap,
                                robject_idx,
                                material_idx,
                                frame->boundary_layers.arr[material_idx]);
                        (*total_num_boundary_layers) += 1;
                }

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

int mliFrame_set_robjects_and_material_map(
        const struct mliFrame *frame,
        struct mliGeometry *geometry,
        struct mliGeometryToMaterialMap *geomap)
{
        uint64_t num_robjects = 0u;
        uint64_t total_num_boundary_layers = 0u;
        mli_check(__mliFrame_set_robjects_and_material_map(
                frame,
                geometry,
                geomap,
                &num_robjects,
                &total_num_boundary_layers),
                "Failed to walk tree of frames to set "
                "robjects and material map.");
        return 1;
error:
        return 0;
}
