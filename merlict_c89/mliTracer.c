/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliTracer.h"
#include <math.h>
#include <stdint.h>
#include "mli_intersection_and_scenery.h"
#include "mli_ray_octree_traversal.h"

struct mliColor mli_trace(
        const struct mliCombine *combine,
        const struct mliRay ray)
{
        struct mliColor color = {128., 128., 128.};
        struct mliIntersectionSurfaceNormal intersection = mliIntersectionSurfaceNormal_init();
        if (mli_first_casual_intersection(combine, ray, &intersection)) {
                struct mliIntersectionSurfaceNormal global_light_intersection;
                struct mliRay line_of_sight_to_source;
                struct mliSide side;
                struct mliSurface surface;
                struct mliVec dir_to_source = {1., 1., 3.};

                line_of_sight_to_source =
                        mliRay_set(intersection.position, dir_to_source);

                side = _mli_side_going_to(combine->scenery, &intersection);
                surface = combine->scenery->resources.surfaces[side.surface];
                color = combine->scenery->resources.colors[surface.color];

                if (mli_first_casual_intersection(
                            combine,
                            line_of_sight_to_source,
                            &global_light_intersection)) {
                        color.r = color.r * .5;
                        color.g = color.g * .5;
                        color.b = color.b * .5;
                }
        }

        return color;
}
