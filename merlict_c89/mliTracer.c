/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliTracer.h"
#include <math.h>
#include <stdint.h>
#include "mli_intersection_and_scenery.h"
#include "mli_ray_octree_traversal.h"

struct mliColor mli_trace(
        const struct mliScenery *scenery,
        const struct mliRay ray)
{
        struct mliColor color = {128., 128., 128.};
        struct mliIntersectionSurfaceNormal intersection =
                mliIntersectionSurfaceNormal_init();
        if (mli_query_intersection_with_surface_normal(
                    scenery, ray, &intersection)) {
                struct mliIntersectionSurfaceNormal global_light_intersection;
                struct mliRay line_of_sight_to_source;
                struct mliSide side;
                struct mliSurface surface;
                struct mliVec dir_to_source = {1., 1., 3.};

                line_of_sight_to_source =
                        mliRay_set(intersection.position, dir_to_source);

                side = _mli_side_going_to(scenery, &intersection);
                surface = scenery->materials.surfaces[side.surface];
                color = scenery->materials.colors[surface.color];

                if (mli_query_intersection_with_surface_normal(
                            scenery,
                            line_of_sight_to_source,
                            &global_light_intersection)) {
                        color.r = color.r * .5;
                        color.g = color.g * .5;
                        color.b = color.b * .5;
                } else {
                        const double theta = mliVec_angle_between(
                                dir_to_source, intersection.surface_normal);
                        const double lambert = fabs(cos(theta));
                        color.r = color.r * 0.5 * (1 + lambert);
                        color.g = color.g * 0.5 * (1 + lambert);
                        color.b = color.b * 0.5 * (1 + lambert);
                }
        }

        return color;
}
