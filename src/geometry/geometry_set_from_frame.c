/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "geometry_set_from_frame.h"
#include "geometry.h"
#include <math.h>
#include "../chk/chk.h"
#include "../frame/frame.h"
#include "../geometrytomaterialmap/geometrytomaterialmap.h"

int mli_Geometry__set_robjects_and_material_map_from_frame_walk(
        const struct mli_Frame *frame,
        struct mli_Geometry *geometry,
        struct mli_GeometryToMaterialMap *geomap,
        uint64_t *num_robjects,
        uint64_t *total_num_boundary_layers)
{
        uint64_t c;
        uint64_t material_idx;
        uint64_t robject_idx;
        switch (frame->type) {
        case MLI_FRAME_TYPE_FRAME:
                for (c = 0; c < frame->children.size; c++) {
                        chk(mli_Geometry__set_robjects_and_material_map_from_frame_walk(
                                frame->children.array[c],
                                geometry,
                                geomap,
                                num_robjects,
                                total_num_boundary_layers));
                }
                break;
        case MLI_FRAME_TYPE_OBJECT:
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
                        mli_GeometryToMaterialMap_set(
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
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

int mli_Geometry_set_robjects_and_material_map_from_frame(
        const struct mli_Frame *frame,
        struct mli_Geometry *geometry,
        struct mli_GeometryToMaterialMap *geomap)
{
        uint64_t num_robjects = 0u;
        uint64_t total_num_boundary_layers = 0u;
        chk_msg(mli_Geometry__set_robjects_and_material_map_from_frame_walk(
                        frame,
                        geometry,
                        geomap,
                        &num_robjects,
                        &total_num_boundary_layers),
                "Failed to walk tree of frames to set "
                "robjects and material map.");
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}
