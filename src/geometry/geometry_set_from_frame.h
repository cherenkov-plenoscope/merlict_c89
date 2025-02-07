/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef GEOMETRY_SET_FROM_FRAME_H_
#define GEOMETRY_SET_FROM_FRAME_H_

#include <stdint.h>
#include "../chk/chk.h"
struct mli_Frame;
struct mli_Geometry;
struct mli_GeometryToMaterialMap;

chk_rc mli_Geometry_set_robjects_and_material_map_from_frame(
        const struct mli_Frame *frame,
        struct mli_Geometry *geometry,
        struct mli_GeometryToMaterialMap *geomap);

chk_rc mli_Geometry__set_robjects_and_material_map_from_frame_walk(
        const struct mli_Frame *frame,
        struct mli_Geometry *geometry,
        struct mli_GeometryToMaterialMap *geomap,
        uint64_t *num_robjects,
        uint64_t *total_num_boundary_layers);

#endif
