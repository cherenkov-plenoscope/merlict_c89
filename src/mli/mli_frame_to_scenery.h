/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_FRAME_TO_SCENERY_H_
#define MLI_FRAME_TO_SCENERY_H_

#include <stdint.h>
struct mli_Frame;
struct mli_Geometry;
struct mli_GeometryToMaterialMap;

int mli_Frame_set_robjects_and_material_map(
        const struct mli_Frame *frame,
        struct mli_Geometry *geometry,
        struct mli_GeometryToMaterialMap *geomap);

int mli_Frame_set_robjects_and_material_map_walk(
        const struct mli_Frame *frame,
        struct mli_Geometry *geometry,
        struct mli_GeometryToMaterialMap *geomap,
        uint64_t *num_robjects,
        uint64_t *total_num_boundary_layers);

int mli_Frame_estimate_num_robjects_and_total_num_boundary_layers(
        const struct mli_Frame *frame,
        uint64_t *num_robjects,
        uint64_t *total_num_boundary_layers);

int mli_Frame_estimate_num_robjects_and_total_num_boundary_layers_walk(
        const struct mli_Frame *frame,
        uint64_t *num_robjects,
        uint64_t *total_num_boundary_layers);

#endif
