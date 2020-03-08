/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLI_INTERSECTION_AND_SCENERY_H_
#define MERLICT_MLI_INTERSECTION_AND_SCENERY_H_

#include <math.h>
#include <stdint.h>
#include "mliIntersection.h"
#include "mliScenery.h"
#include "mliPhotonInteraction.h"

struct mliSide _mli_side_coming_from(
    const struct mliScenery *scenery,
    const struct mliIntersection *isec)
{
    struct mliSurfaces surfaces = mliScenery_object_surfaces(
        scenery,
        isec->object_idx);
    if (isec->from_outside_to_inside)
        return surfaces.inner;
    else
        return surfaces.outer;
}

struct mliSide _mli_side_going_to(
    const struct mliScenery *scenery,
    const struct mliIntersection *isec)
{
    struct mliSurfaces surfaces = mliScenery_object_surfaces(
        scenery,
        isec->object_idx);
    if (isec->from_outside_to_inside)
        return surfaces.outer;
    else
        return surfaces.inner;
}

const struct mliFunc *_mli_refractive_index_going_to(
    const struct mliScenery *scenery,
    const struct mliIntersection *isec)
{
    const struct mliFunc *refractive_index;
    const struct mliSide going_to = _mli_side_going_to(scenery, isec);
    const struct mliMedium medium = scenery->media[going_to.medium];
    refractive_index = &scenery->functions[medium.refraction];
    return refractive_index;
}

const struct mliFunc *_mli_refractive_index_coming_from(
    const struct mliScenery *scenery,
    const struct mliIntersection *isec)
{
    const struct mliFunc *refractive_index;
    const struct mliSide coming_from = _mli_side_coming_from(scenery, isec);
    const struct mliMedium medium = scenery->media[coming_from.medium];
    refractive_index = &scenery->functions[medium.refraction];
    return refractive_index;
}

struct mliIntersection mliIntersection_photon_creation(
    const uint64_t object_idx,
    const struct mliRay ray)
{
    struct mliIntersection isec;
    isec.object_idx = object_idx;
    isec.position = ray.support;
    isec.surface_normal = ray.direction;
    isec.distance_of_ray = 0.0;
    isec.from_outside_to_inside = 1;
    return isec;
}

#endif
