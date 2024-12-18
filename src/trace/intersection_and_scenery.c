/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "intersection_and_scenery.h"

struct mli_boundarylayer_Side mli_trace_get_side_coming_from(
        const struct mli_Scenery *scenery,
        const struct mli_IntersectionSurfaceNormal *isec)
{
        struct mli_BoundaryLayer layer =
                scenery->materials
                        .boundary_layers[mli_Scenery_resolve_boundary_layer_idx(
                                scenery, isec->geometry_id)];
        if (isec->from_outside_to_inside)
                return layer.outer;
        else
                return layer.inner;
}

struct mli_boundarylayer_Side mli_trace_get_side_going_to(
        const struct mli_Scenery *scenery,
        const struct mli_IntersectionSurfaceNormal *isec)
{
        struct mli_BoundaryLayer layer =
                scenery->materials
                        .boundary_layers[mli_Scenery_resolve_boundary_layer_idx(
                                scenery, isec->geometry_id)];
        if (isec->from_outside_to_inside)
                return layer.inner;
        else
                return layer.outer;
}

const struct mli_Func *mli_trace_get_refractive_index_going_to(
        const struct mli_Scenery *scenery,
        const struct mli_IntersectionSurfaceNormal *isec)
{
        const struct mli_boundarylayer_Side going_to =
                mli_trace_get_side_going_to(scenery, isec);
        return &scenery->materials.media[going_to.medium].refraction;
}

const struct mli_Func *mli_trace_get_refractive_index_coming_from(
        const struct mli_Scenery *scenery,
        const struct mli_IntersectionSurfaceNormal *isec)
{
        const struct mli_boundarylayer_Side coming_from =
                mli_trace_get_side_coming_from(scenery, isec);
        return &scenery->materials.media[coming_from.medium].refraction;
}
