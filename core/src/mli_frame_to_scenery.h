/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_FRAME_TO_SCENERY_H_
#define MLI_FRAME_TO_SCENERY_H_

#include <stdint.h>
#include "mliGeometry.h"

int mliFrame_set_robjects_and_material_map(
        const struct mliFrame *frame,
        struct mliGeometry *geometry,
        struct mliGeometryToMaterialMap *geomap);

int mliFrame_set_robjects_and_material_map_walk(
        const struct mliFrame *frame,
        struct mliGeometry *geometry,
        struct mliGeometryToMaterialMap *geomap,
        uint64_t *num_robjects,
        uint64_t *total_num_boundary_layers);

int mliFrame_estimate_num_robjects_and_total_num_boundary_layers(
        const struct mliFrame *frame,
        uint64_t *num_robjects,
        uint64_t *total_num_boundary_layers);

int mliFrame_estimate_num_robjects_and_total_num_boundary_layers_walk(
        const struct mliFrame *frame,
        uint64_t *num_robjects,
        uint64_t *total_num_boundary_layers);

#endif
