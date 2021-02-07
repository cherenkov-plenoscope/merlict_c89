/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mli_intersection_and_scenery.h"

struct mliSide _mli_side_coming_from(
        const struct mliScenery *scenery,
        const struct mliIntersectionSurfaceNormal *isec)
{
        struct mliBoundaryLayer layer =
                scenery->robject_boundary_layers[isec->geometry_id.robj];
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
                scenery->robject_boundary_layers[isec->geometry_id.robj];
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
        const struct mliMedium medium =
                scenery->resources.media[going_to.medium];
        refractive_index = &scenery->resources.functions[medium.refraction];
        return refractive_index;
}

const struct mliFunc *_mli_refractive_index_coming_from(
        const struct mliScenery *scenery,
        const struct mliIntersectionSurfaceNormal *isec)
{
        const struct mliFunc *refractive_index;
        const struct mliSide coming_from = _mli_side_coming_from(scenery, isec);
        const struct mliMedium medium =
                scenery->resources.media[coming_from.medium];
        refractive_index = &scenery->resources.functions[medium.refraction];
        return refractive_index;
}
