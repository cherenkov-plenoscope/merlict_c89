/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_frame_to_scenery.h"
#include <math.h>

int mliFrame_estimate_num_robjects_and_total_num_boundary_layers_walk(
        const struct mliFrame *frame,
        uint64_t *num_robjects,
        uint64_t *total_num_boundary_layers)
{
        uint64_t c;
        switch (frame->type) {
        case MLI_FRAME:
                for (c = 0; c < frame->children.size; c++) {
                        chk(mliFrame_estimate_num_robjects_and_total_num_boundary_layers_walk(
                                frame->children.array[c],
                                num_robjects,
                                total_num_boundary_layers));
                }
                break;
        case MLI_OBJECT:
                (*num_robjects) += 1;
                (*total_num_boundary_layers) += frame->boundary_layers.size;
                break;
        default:
                chk_bad("Expected either type 'frame' or 'object'.");
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
        chk_msg(mliFrame_estimate_num_robjects_and_total_num_boundary_layers_walk(
                        frame, num_robjects, total_num_boundary_layers),
                "Failed to walk tree of frames to estimate "
                "num_robjects and total_num_boundary_layers.");
        return 1;
error:
        return 0;
}

int mliFrame_set_robjects_and_material_map_walk(
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
                for (c = 0; c < frame->children.size; c++) {
                        chk(mliFrame_set_robjects_and_material_map_walk(
                                frame->children.array[c],
                                geometry,
                                geomap,
                                num_robjects,
                                total_num_boundary_layers));
                }
                break;
        case MLI_OBJECT:
                robject_idx = (*num_robjects);

                chk_msg(frame->object < geometry->num_objects,
                        "Expected frame->object < num_objects.");
                /* geometry */
                geometry->robjects[robject_idx] = frame->object;
                geometry->robject2root[robject_idx] = frame->frame2root;
                geometry->robject_ids[robject_idx] = frame->id;

                /* materials map */
                chk_msg(frame->boundary_layers.size ==
                                geometry->objects[frame->object].num_materials,
                        "Expected Frame to have same "
                        "num boundary_layers as object.");

                geomap->first_boundary_layer_in_robject[robject_idx] =
                        (*total_num_boundary_layers);
                for (material_idx = 0;
                     material_idx < frame->boundary_layers.size;
                     material_idx++) {
                        mliGeometryToMaterialMap_set(
                                geomap,
                                robject_idx,
                                material_idx,
                                frame->boundary_layers.array[material_idx]);
                        (*total_num_boundary_layers) += 1;
                }

                (*num_robjects) += 1;
                break;
        default:
                chk_bad("Expected either type 'frame' or 'object'.");
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
        chk_msg(mliFrame_set_robjects_and_material_map_walk(
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
