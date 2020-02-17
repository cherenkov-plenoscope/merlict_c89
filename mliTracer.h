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
#include "mli_intersection_and_scenery.h"

int mli_first_casual_intersection(
    const mliScenery *scenery,
    const mliOcTree* octree,
    const struct mliRay ray,
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

struct mliColor mli_trace(
    const mliScenery *scenery,
    const mliOcTree* octree,
    const struct mliRay ray) {
    struct mliColor color = {128., 128., 128.};
    mliIntersection intersection;
    if (
        mli_first_casual_intersection(
            scenery,
            octree,
            ray,
            &intersection)
    ) {
        mliIntersection global_light_intersection;
        struct mliRay line_of_sight_to_source;
        mliSurface surface;
        struct mliVec dir_to_source = {1., 1., 3.};

        line_of_sight_to_source = mliRay_set(
            intersection.position,
            dir_to_source);

        surface = _mli_surface_going_to(scenery, &intersection);
        color = scenery->colors[surface.color];

        if (mli_first_casual_intersection(
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
