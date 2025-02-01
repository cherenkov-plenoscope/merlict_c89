/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_RAYTRACING_INTERSECTION_AND_SCENERY_H_
#define MLI_RAYTRACING_INTERSECTION_AND_SCENERY_H_

#include "../intersection/intersection_surface_normal.h"
#include "../scenery/scenery.h"

struct mli_IntersectionLayerSide {
        const struct mli_Surface *surface;
        uint64_t surface_idx;
        const struct mli_BoundaryLayer_Medium *medium;
        uint64_t medium_idx;
};

struct mli_IntersectionLayerSide mli_IntersectionLayerSide_init(void);

struct mli_IntersectionLayer {
        struct mli_IntersectionLayerSide side_coming_from;
        struct mli_IntersectionLayerSide side_going_to;
};

struct mli_IntersectionLayer mli_IntersectionLayer_init(void);

struct mli_IntersectionLayer mli_raytracing_get_intersection_layer(
        const struct mli_Scenery *scenery,
        const struct mli_IntersectionSurfaceNormal *isec);

struct mli_BoundaryLayer_Side mli_raytracing_get_side_going_to(
        const struct mli_Scenery *scenery,
        const struct mli_IntersectionSurfaceNormal *isec);
struct mli_BoundaryLayer_Side mli_raytracing_get_side_coming_from(
        const struct mli_Scenery *scenery,
        const struct mli_IntersectionSurfaceNormal *isec);

const struct mli_Func *mli_raytracing_get_refractive_index_coming_from(
        const struct mli_Scenery *scenery,
        const struct mli_IntersectionSurfaceNormal *isec);
const struct mli_Func *mli_raytracing_get_refractive_index_going_to(
        const struct mli_Scenery *scenery,
        const struct mli_IntersectionSurfaceNormal *isec);

#endif
