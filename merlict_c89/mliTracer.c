/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliTracer.h"
#include <math.h>
#include <stdint.h>
#include "mli_intersection_and_scenery.h"
#include "mli_ray_octree_traversal.h"
#include "mliMT19937.h"

struct mliColor _trace_to_background(const struct mliTracerCongig *config)
{
        return config->background_color;
}

double _mli_shadowing(
        const struct mliTracerCongig *config,
        const struct mliVec position,
        const struct mliScenery *scenery,
        struct mliMT19937 *prng)
{
        uint64_t i;
        double num_obstructions = 0.0;

        for (i = 0; i < config->num_trails_global_light_source; i++) {
                struct mliVec pos_in_source = mliVec_add(
                        config->global_light_source.position,
                        mliVec_multiply(
                                mli_random_position_inside_unit_sphere(prng),
                                config->global_light_source.radius));

                struct mliRay line_of_sight_to_source = mliRay_set(
                        position, mliVec_substract(pos_in_source, position));

                struct mliIntersectionMinimalQuery isec;

                mli_query_intersection_minimal(
                        scenery, line_of_sight_to_source, &isec);

                if (isec.distance_of_ray != DBL_MAX) {
                        num_obstructions += 1.0;
                }
        }

        return num_obstructions / config->num_trails_global_light_source;
}

struct mliColor _trace_to_intersection(
        const struct mliTracerCongig *config,
        const struct mliIntersectionSurfaceNormal *intersection,
        const struct mliScenery *scenery,
        struct mliMT19937 *prng)
{
        struct mliColor color;
        struct mliSide side;
        struct mliSurface surface;

        const double shadow =
                _mli_shadowing(config, intersection->position, scenery, prng);

        side = _mli_side_going_to(scenery, intersection);
        surface = scenery->materials.surfaces[side.surface];
        color = scenery->materials.colors[surface.color];

        const double theta = mliVec_angle_between(
                mliVec_substract(
                        config->global_light_source.position,
                        intersection->position),
                intersection->surface_normal);
        const double lambert = fabs(cos(theta));

        color.r = color.r * 0.5 * (1.0 + (1.0 - shadow) * lambert);
        color.g = color.g * 0.5 * (1.0 + (1.0 - shadow) * lambert);
        color.b = color.b * 0.5 * (1.0 + (1.0 - shadow) * lambert);

        return color;
}

struct mliColor mli_trace(
        const struct mliScenery *scenery,
        const struct mliRay ray,
        struct mliMT19937 *prng)
{
        struct mliTracerCongig config = mliTracerCongig_init();

        struct mliIntersectionSurfaceNormal intersection =
                mliIntersectionSurfaceNormal_init();

        if (mli_query_intersection_with_surface_normal(
                    scenery, ray, &intersection)) {
                return _trace_to_intersection(
                        &config, &intersection, scenery, prng);
        } else {
                return _trace_to_background(&config);
        }
}

struct mliTracerCongig mliTracerCongig_init(void)
{
        double distance_earth_to_sun = 1.5e11;
        double radius_sun = 7e8;

        struct mliTracerCongig config;
        config.background_color = mliColor_set(128.0, 128.0, 128.0);

        config.ambient_power = 0.5;

        config.global_light_source.position = mliVec_multiply(
                mliVec_normalized(mliVec_set(1.0, 1.0, 3.0)),
                distance_earth_to_sun);
        config.global_light_source.radius = radius_sun;
        config.global_light_source.distance_in_default_medium = 1.0;
        config.global_light_source.power = 0.5;
        config.num_trails_global_light_source = 10;

        return config;
}
