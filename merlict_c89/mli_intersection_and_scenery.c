/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_intersection_and_scenery.h"

uint32_t mliScenery_resolve_boundary_layer_idx(
        const struct mliScenery *scenery,
        const struct mliGeometryId geometry_id)
{
        const uint32_t robject_idx = geometry_id.robj;
        const uint32_t object_idx = scenery->geometry.robjects[robject_idx];
        const uint32_t face_idx = geometry_id.face;
        const uint32_t obj_mtl_idx = mliObject_resolve_material_idx(
                &scenery->geometry.objects[object_idx],
                face_idx);
        const uint32_t boundary_layer_idx = mliGeometryToMaterialMap_get(
                &scenery->geomap,
                robject_idx,
                obj_mtl_idx);
        return boundary_layer_idx;
}

struct mliSide _mli_side_coming_from(
        const struct mliScenery *scenery,
        const struct mliIntersectionSurfaceNormal *isec)
{
        struct mliBoundaryLayer layer = scenery->materials.boundary_layers[
                mliScenery_resolve_boundary_layer_idx(
                        scenery,
                        isec->geometry_id)
        ];
        if (isec->from_outside_to_inside)
                return layer.inner;
        else
                return layer.outer;
}

struct mliSide _mli_side_going_to(
        const struct mliScenery *scenery,
        const struct mliIntersectionSurfaceNormal *isec)
{
        struct mliBoundaryLayer layer = scenery->materials.boundary_layers[
                mliScenery_resolve_boundary_layer_idx(
                        scenery,
                        isec->geometry_id)
        ];
        if (isec->from_outside_to_inside)
                return layer.outer;
        else
                return layer.inner;
}

const struct mliFunc *_mli_refractive_index_going_to(
        const struct mliScenery *scenery,
        const struct mliIntersectionSurfaceNormal *isec)
{
        const struct mliFunc *refractive_index;
        const struct mliSide going_to = _mli_side_going_to(scenery, isec);
        const struct mliMedium medium = scenery->materials.media[
                going_to.medium];
        refractive_index = &scenery->materials.functions[medium.refraction];
        return refractive_index;
}

const struct mliFunc *_mli_refractive_index_coming_from(
        const struct mliScenery *scenery,
        const struct mliIntersectionSurfaceNormal *isec)
{
        const struct mliFunc *refractive_index;
        const struct mliSide coming_from = _mli_side_coming_from(scenery, isec);
        const struct mliMedium medium = scenery->materials.media[
                coming_from.medium];
        refractive_index = &scenery->materials.functions[medium.refraction];
        return refractive_index;
}
