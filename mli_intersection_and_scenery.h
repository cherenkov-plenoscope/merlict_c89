/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLI_INTERSECTION_AND_SCENERY_H_
#define MERLICT_MLI_INTERSECTION_AND_SCENERY_H_

#include <math.h>
#include <stdint.h>
#include "mliIntersection.h"
#include "mliScenery.h"
#include "mliPhotonInteraction.h"

struct mliSurface _mli_surface_coming_from(
    const struct mliScenery *scenery,
    const struct mliIntersection *isec) {
    mliSurfaces surfaces = mliScenery_object_surfaces(
        scenery,
        isec->object_idx);
    if (isec->from_outside_to_inside)
        return scenery->surfaces[surfaces.inner];
    else
        return scenery->surfaces[surfaces.outer];}

struct mliSurface _mli_surface_going_to(
    const struct mliScenery *scenery,
    const struct mliIntersection *isec) {
    mliSurfaces surfaces = mliScenery_object_surfaces(
        scenery,
        isec->object_idx);
    if (isec->from_outside_to_inside)
        return scenery->surfaces[surfaces.outer];
    else
        return scenery->surfaces[surfaces.inner];}

const struct mliFunc *_mli_refractive_index_going_to(
    const struct mliScenery *scenery,
    const struct mliIntersection *isec) {
    const struct mliFunc *refractive_index;
    const struct mliSurface going_to = _mli_surface_going_to(scenery, isec);
    refractive_index = &scenery->functions[going_to.medium_refraction];
    return refractive_index;}

const struct mliFunc *_mli_refractive_index_coming_from(
    const struct mliScenery *scenery,
    const struct mliIntersection *isec) {
    const struct mliFunc *refractive_index;
    const struct mliSurface coming_from = _mli_surface_coming_from(scenery, isec);
    refractive_index = &scenery->functions[coming_from.medium_refraction];
    return refractive_index;}

struct mliIntersection mliIntersection_photon_creation(
    const uint64_t object_idx,
    const struct mliRay ray) {
    struct mliIntersection isec;
    isec.object_idx = object_idx;
    isec.position = ray.support;
    isec.surface_normal = ray.direction;
    isec.distance_of_ray = 0.0;
    isec.from_outside_to_inside = 1;
    return isec;}

#endif
