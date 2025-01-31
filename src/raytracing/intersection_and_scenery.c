/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "intersection_and_scenery.h"

struct mli_BoundaryLayer_Side mli_raytracing_get_side_coming_from(
        const struct mli_Scenery *scenery,
        const struct mli_IntersectionSurfaceNormal *isec)
{
        const uint64_t idx = mli_Scenery_resolve_boundary_layer_idx(
                scenery, isec->geometry_id);
        struct mli_BoundaryLayer2 layer = scenery->materials.layers2.array[idx];
        if (isec->from_outside_to_inside)
                return layer.outer;
        else
                return layer.inner;
}

struct mli_BoundaryLayer_Side mli_raytracing_get_side_going_to(
        const struct mli_Scenery *scenery,
        const struct mli_IntersectionSurfaceNormal *isec)
{
        const uint64_t idx = mli_Scenery_resolve_boundary_layer_idx(
                scenery, isec->geometry_id);
        struct mli_BoundaryLayer2 layer = scenery->materials.layers2.array[idx];
        if (isec->from_outside_to_inside)
                return layer.inner;
        else
                return layer.outer;
}

const struct mli_Func *mli_raytracing_get_refractive_index_going_to(
        const struct mli_Scenery *scenery,
        const struct mli_IntersectionSurfaceNormal *isec)
{
        const struct mli_BoundaryLayer_Side going_to =
                mli_raytracing_get_side_going_to(scenery, isec);
        const uint64_t idx = scenery->materials.media2.array[going_to.medium]
                                     .refraction_spectrum;
        return &scenery->materials.spectra.array[idx].spectrum;
}

const struct mli_Func *mli_raytracing_get_refractive_index_coming_from(
        const struct mli_Scenery *scenery,
        const struct mli_IntersectionSurfaceNormal *isec)
{
        const struct mli_BoundaryLayer_Side coming_from =
                mli_raytracing_get_side_coming_from(scenery, isec);
        const uint64_t idx = scenery->materials.media2.array[coming_from.medium]
                                     .refraction_spectrum;
        return &scenery->materials.spectra.array[idx].spectrum;
}
