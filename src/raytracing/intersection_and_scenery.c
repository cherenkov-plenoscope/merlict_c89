/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "intersection_and_scenery.h"

struct mli_IntersectionLayerSide mli_IntersectionLayerSide_init(void)
{
        struct mli_IntersectionLayerSide side;
        side.surface = NULL;
        side.surface_idx = 0;
        side.medium = NULL;
        side.medium_idx = 0;
        return side;
}

struct mli_IntersectionLayer mli_IntersectionLayer_init(void)
{
        struct mli_IntersectionLayer ilay;
        ilay.side_coming_from = mli_IntersectionLayerSide_init();
        ilay.side_going_to = mli_IntersectionLayerSide_init();
        return ilay;
}

struct mli_IntersectionLayer mli_raytracing_get_intersection_layer(
        const struct mli_Scenery *scenery,
        const struct mli_IntersectionSurfaceNormal *isec)
{
        const uint64_t idx = mli_Scenery_resolve_boundary_layer_idx(
                scenery, isec->geometry_id);
        const struct mli_BoundaryLayer2 layer =
                scenery->materials.boundary_layers.array[idx];
        struct mli_IntersectionLayer ilay = mli_IntersectionLayer_init();

        const struct mli_BoundaryLayer_Surface *inner_surface =
                &scenery->materials.surfaces.array[layer.inner.surface];
        const struct mli_BoundaryLayer_Medium *inner_medium =
                &scenery->materials.media.array[layer.inner.medium];
        const struct mli_BoundaryLayer_Surface *outer_surface =
                &scenery->materials.surfaces.array[layer.outer.surface];
        const struct mli_BoundaryLayer_Medium *outer_medium =
                &scenery->materials.media.array[layer.outer.medium];

        if (isec->from_outside_to_inside) {
                ilay.side_coming_from.surface = outer_surface;
                ilay.side_coming_from.medium = outer_medium;
                ilay.side_coming_from.surface_idx = layer.outer.surface;
                ilay.side_coming_from.medium_idx = layer.outer.medium;

                ilay.side_going_to.surface = inner_surface;
                ilay.side_going_to.medium = inner_medium;
                ilay.side_going_to.surface_idx = layer.inner.surface;
                ilay.side_going_to.medium_idx = layer.inner.medium;
        } else {
                ilay.side_coming_from.surface = inner_surface;
                ilay.side_coming_from.medium = inner_medium;
                ilay.side_coming_from.surface_idx = layer.inner.surface;
                ilay.side_coming_from.medium_idx = layer.inner.medium;

                ilay.side_going_to.surface = outer_surface;
                ilay.side_going_to.medium = outer_medium;
                ilay.side_going_to.surface_idx = layer.outer.surface;
                ilay.side_going_to.medium_idx = layer.outer.medium;
        }

        return ilay;
}

struct mli_BoundaryLayer_Side mli_raytracing_get_side_coming_from(
        const struct mli_Scenery *scenery,
        const struct mli_IntersectionSurfaceNormal *isec)
{
        const uint64_t idx = mli_Scenery_resolve_boundary_layer_idx(
                scenery, isec->geometry_id);
        struct mli_BoundaryLayer2 layer =
                scenery->materials.boundary_layers.array[idx];
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
        struct mli_BoundaryLayer2 layer =
                scenery->materials.boundary_layers.array[idx];
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
        const uint64_t idx = scenery->materials.media.array[going_to.medium]
                                     .refraction_spectrum;
        return &scenery->materials.spectra.array[idx].spectrum;
}

const struct mli_Func *mli_raytracing_get_refractive_index_coming_from(
        const struct mli_Scenery *scenery,
        const struct mli_IntersectionSurfaceNormal *isec)
{
        const struct mli_BoundaryLayer_Side coming_from =
                mli_raytracing_get_side_coming_from(scenery, isec);
        const uint64_t idx = scenery->materials.media.array[coming_from.medium]
                                     .refraction_spectrum;
        return &scenery->materials.spectra.array[idx].spectrum;
}
