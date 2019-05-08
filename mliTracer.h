/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLITRACER_H_
#define MERLICT_MLITRACER_H_

#include <math.h>
#include <stdint.h>
#include "mliVec.h"
#include "mliRay.h"
#include "mliHomTra.h"
#include "mliColor.h"
#include "mliFunc.h"
#include "mliScenery.h"
#include "mliScenery_object_interface.h"
#include "mliIntersection.h"
#include "mliOcTree.h"
#include "mli_ray_octree_traversal.h"

/*
int first_casual_intersection(
    const mliScenery *scenery,
    const mliRay *ray,
    const int64_t face_coming_from,
    mliIntersection *intersection) {
    int64_t hit = 0;
    int64_t idx;
    int64_t num_objects;

    mliIntersection closest_isec;
    mliIntersection temp_isec;
    temp_isec.distance_of_ray = 9e99;
    closest_isec.distance_of_ray = 9e99;
    num_objects = mliScenery_num_objects(scenery);
    for (idx = 0; idx < num_objects; idx++) {
        if (face_coming_from == idx)
            continue;
        if (
            mliScenery_intersection(
                scenery,
                (*ray),
                idx,
                &temp_isec)
        ) {
            hit = hit + 1;
            if (temp_isec.distance_of_ray < closest_isec.distance_of_ray) {
                closest_isec = temp_isec;
            }
        }
    }
    if (hit>0) {
        (*intersection) = closest_isec;
    }
    return hit;
}*/

int first_casual_intersection(
    const mliScenery *scenery,
    const mliOcTree* octree,
    const mliRay ray,
    const int64_t face_coming_from,
    mliIntersection *intersection) {
    mli_ray_octree_traversal(
        scenery,
        octree,
        ray,
        intersection,
        face_coming_from);
    if (intersection->distance_of_ray < FLT_MAX)
        return 1;
    else
        return 0;
}



mliColor mli_trace(
    const mliScenery *scenery,
    const mliOcTree* octree,
    const mliRay ray) {
    mliColor color = {128., 128., 128.};
    mliIntersection intersection;
    if (
        first_casual_intersection(
            scenery,
            octree,
            ray,
            mliScenery_num_objects(scenery),
            &intersection)
    ) {
        mliIntersection global_light_intersection;
        mliRay line_of_sight_to_source;
        mliSurfaces surfaces;
        mliSurface outer_surface;
        mliVec dir_to_source = {1., 1., -3.};

        line_of_sight_to_source = mliRay_set(
            intersection.position,
            dir_to_source);

        surfaces = mliScenery_object_surfaces(
            scenery,
            intersection.object_idx);

        outer_surface = scenery->surfaces[surfaces.outer];
        color = scenery->colors[outer_surface.color];

        if (first_casual_intersection(
                scenery,
                octree,
                line_of_sight_to_source,
                intersection.object_idx,
                &global_light_intersection)
        ) {
            color.r = color.r*.5;
            color.g = color.g*.5;
            color.b = color.b*.5;
        }
    }

    return color;
}

#endif
