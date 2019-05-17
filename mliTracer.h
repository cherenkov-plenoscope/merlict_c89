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


int first_casual_intersection(
    const mliScenery *scenery,
    const mliOcTree* octree,
    const mliRay ray,
    mliIntersection *intersection) {
    mli_ray_octree_traversal(
        scenery,
        octree,
        ray,
        intersection);
    if (intersection->distance_of_ray < DBL_MAX)
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
            &intersection)
    ) {
        mliIntersection global_light_intersection;
        mliRay line_of_sight_to_source;
        mliSurfaces surfaces;
        mliSurface outer_surface;
        mliVec dir_to_source = {1., 1., 3.};

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
