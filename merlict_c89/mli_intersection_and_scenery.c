/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_intersection_and_scenery.h"

struct mliSide _mli_side_coming_from(
        const struct mliScenery *scenery,
        const struct mliIntersectionSurfaceNormal *isec)
{
        /*
        const uint32_t robject_idx = isec->geometry_id.robj;
        const uint32_t object_idx = scenery->geometry.robjects[robject_idx];
        const uint32_t face_idx = isec->geometry_id.face;
        const uint32_t obj_mtl_idx = mliObject_resolve_material_idx(
                &scenery->geometry.objects[object_idx],
                face_idx);
        */

        struct mliBoundaryLayer layer =
                scenery->geometry.robject_boundary_layers[isec->geometry_id.robj];
        if (isec->from_outside_to_inside)
                return layer.inner;
        else
                return layer.outer;
}

struct mliSide _mli_side_going_to(
        const struct mliScenery *scenery,
        const struct mliIntersectionSurfaceNormal *isec)
{
        struct mliBoundaryLayer layer =
                scenery->geometry.robject_boundary_layers[isec->geometry_id.robj];
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
