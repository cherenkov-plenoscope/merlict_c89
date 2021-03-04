/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliTracer.h"
#include "mliTracer_atmosphere.c"
#include <math.h>
#include <stdint.h>
#include "mli_intersection_and_scenery.h"
#include "mli_ray_octree_traversal.h"

double _mli_trace_sun_visibility(
        const struct mliScenery *scenery,
        const struct mliVec position,
        const struct mliTracerCongig *config,
        struct mliMT19937 *prng)
{
        return (
                1.0 -
                _mli_trace_sun_obstruction(scenery, position, config, prng)
        );
}

double _mli_trace_sun_obstruction(
        const struct mliScenery *scenery,
        const struct mliVec position,
        const struct mliTracerCongig *config,
        struct mliMT19937 *prng)
{
        uint64_t i;
        double num_obstructions = 0.0;

        for (i = 0; i < config->num_trails_global_light_source; i++) {
                struct mliVec pos_in_source = mliVec_add(
                        mliVec_multiply(
                                config->atmosphere.sunDirection,
                                config->atmosphere.sunDistance
                        ),
                        mliVec_multiply(
                                mli_random_position_inside_unit_sphere(prng),
                                config->atmosphere.sunRadius
                        )
                );

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
        double theta;
        double lambert_factor;

        const double sun_visibility = _mli_trace_sun_visibility(
                    scenery, intersection->position, config, prng);

        side = _mli_side_coming_from(scenery, intersection);
        surface = scenery->materials.surfaces[side.surface];
        color = scenery->materials.colors[surface.color];

        theta = mliVec_angle_between(
                config->atmosphere.sunDirection,
                intersection->surface_normal);
        lambert_factor = fabs(cos(theta));

        color.r = color.r * 0.5 * (1.0 + sun_visibility * lambert_factor);
        color.g = color.g * 0.5 * (1.0 + sun_visibility * lambert_factor);
        color.b = color.b * 0.5 * (1.0 + sun_visibility * lambert_factor);

        return color;
}

struct mliColor _mli_trace(
        const struct mliScenery *scenery,
        const struct mliRay ray,
        const struct mliTracerCongig *config,
        struct mliMT19937 *prng)
{
        struct mliIntersectionSurfaceNormal intersection =
                mliIntersectionSurfaceNormal_init();

        if (mli_query_intersection_with_surface_normal(
                    scenery, ray, &intersection)) {
                return _trace_to_intersection(
                        config, &intersection, scenery, prng);
        } else {
                return config->background_color;
        }
}

struct mliColor mli_trace(
        const struct mliScenery *scenery,
        const struct mliRay ray,
        const struct mliTracerCongig *config,
        struct mliMT19937 *prng)
{
        if (config->have_atmosphere) {
                return _mli_trace_atmosphere(scenery, ray, config, prng);
        } else {
                return _mli_trace(scenery, ray, config, prng);
        }
}

struct mliTracerCongig mliTracerCongig_init(void)
{
        struct mliTracerCongig config;
        config.background_color = mliColor_set(128.0, 128.0, 128.0);
        config.num_trails_global_light_source = 3;
        config.have_atmosphere = 0;
        config.atmosphere = mliAtmosphere_init();
        return config;
}
