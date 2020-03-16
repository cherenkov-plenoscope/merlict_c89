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
    const struct mliScenery *scenery,
    const struct mliTmpOcTree *octree,
    const struct mliRay ray,
    struct mliIntersection *intersection)
{
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
    const struct mliScenery *scenery,
    const struct mliTmpOcTree *octree,
    const struct mliRay ray)
{
    struct mliColor color = {128., 128., 128.};
    struct mliIntersection intersection;
    if (
        mli_first_casual_intersection(
            scenery,
            octree,
            ray,
            &intersection)
    ) {
        struct mliIntersection global_light_intersection;
        struct mliRay line_of_sight_to_source;
        struct mliSide side;
        struct mliSurface surface;
        struct mliVec dir_to_source = {1., 1., 3.};

        line_of_sight_to_source = mliRay_set(
            intersection.position,
            dir_to_source);

        side = _mli_side_going_to(scenery, &intersection);
        surface = scenery->resources.surfaces[side.surface];
        color = scenery->resources.colors[surface.color];

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
