/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliTracer.h"
#include <math.h>
#include <stdint.h>

struct mliVec mli_random_direction_in_hemisphere(
        struct mliPrng *prng,
        struct mliVec normal)
{
        struct mliVec rnd_dir;
        do {
                rnd_dir = mli_random_position_inside_unit_sphere(prng);
        } while (mliVec_dot(normal, rnd_dir) <= 0.0);
        return mliVec_normalized(rnd_dir);
}

struct mliColor mli_trace_color_tone_of_sun(
        const struct mliTracerCongig *config,
        const struct mliVec support)
{
        struct mliColor sun_color = mliColor_set(1.0, 1.0, 1.0);
        double width_atmosphere = config->atmosphere.atmosphereRadius -
                                  config->atmosphere.earthRadius;

        if (config->atmosphere.altitude < width_atmosphere) {
                struct mliColor color_close_to_sun = mliAtmosphere_query(
                        &config->atmosphere,
                        support,
                        config->atmosphere.sunDirection);

                double f = config->atmosphere.sunDirection.z;
                double max = MLI_MAX3(
                        color_close_to_sun.r,
                        color_close_to_sun.g,
                        color_close_to_sun.b);

                color_close_to_sun.r /= max;
                color_close_to_sun.g /= max;
                color_close_to_sun.b /= max;

                return mliColor_add(
                        mliColor_multiply(sun_color, f),
                        mliColor_multiply(color_close_to_sun, (1.0 - f)));
        } else {
                return sun_color;
        }
}

struct mliColor mli_trace_color_tone_of_diffuse_sky(
        const struct mliTracerCongig *config,
        const struct mliIntersectionSurfaceNormal *intersection,
        const struct mliScenery *scenery,
        struct mliPrng *prng)
{
        int i;
        struct mliColor sky = mliColor_set(0.0, 0.0, 0.0);
        struct mliRay obstruction_ray;
        struct mliVec facing_surface_normal;
        struct mliIntersection isec;
        int has_direct_view_to_sky = 0;
        int num_samples = 5;

        facing_surface_normal =
                intersection->from_outside_to_inside
                        ? intersection->surface_normal
                        : mliVec_multiply(intersection->surface_normal, -1.0);

        for (i = 0; i < num_samples; i++) {
                struct mliVec rnd_dir = mli_random_direction_in_hemisphere(
                        prng, facing_surface_normal);

                obstruction_ray.support = intersection->position;
                obstruction_ray.direction = rnd_dir;

                has_direct_view_to_sky = !mli_query_intersection(
                        scenery, obstruction_ray, &isec);

                if (has_direct_view_to_sky) {
                        struct mliColor sample = mliAtmosphere_query(
                                &config->atmosphere,
                                intersection->position,
                                rnd_dir);

                        double theta = mliVec_angle_between(
                                rnd_dir, facing_surface_normal);
                        double lambert_factor = fabs(cos(theta));

                        sky = mliColor_add(
                                sky, mliColor_multiply(sample, lambert_factor));
                }
        }

        return mliColor_multiply(sky, 1.0 / (255.0 * num_samples));
}

struct mliColor mli_trace_to_intersection_atmosphere(
        const struct mliTracerCongig *config,
        const struct mliIntersectionSurfaceNormal *intersection,
        const struct mliScenery *scenery,
        struct mliPrng *prng)
{
        struct mliColor color;
        struct mliColor tone;
        struct mliSide side;
        double theta;
        double lambert_factor;

        const double sun_visibility = mli_trace_sun_visibility(
                scenery, intersection->position, config, prng);

        if (sun_visibility > 0.0) {
                tone = mli_trace_color_tone_of_sun(
                        config, intersection->position);
                tone = mliColor_multiply(tone, sun_visibility);
        } else {
                tone = mli_trace_color_tone_of_diffuse_sky(
                        config, intersection, scenery, prng);
        }

        side = mli_get_side_coming_from(scenery, intersection);
        color = scenery->materials.surfaces[side.surface].color;

        theta = mliVec_angle_between(
                config->atmosphere.sunDirection, intersection->surface_normal);
        lambert_factor = fabs(cos(theta));

        color = mliColor_multiply(color, lambert_factor);

        return mliColor_multiply_elementwise(color, tone);
}

struct mliColor mli_trace_with_atmosphere(
        const struct mliScenery *scenery,
        const struct mliRay ray,
        const struct mliTracerCongig *config,
        struct mliPrng *prng)
{
        struct mliIntersectionSurfaceNormal intersection =
                mliIntersectionSurfaceNormal_init();
        struct mliColor out;

        if (mli_query_intersection_with_surface_normal(
                    scenery, ray, &intersection)) {
                out = mli_trace_to_intersection_atmosphere(
                        config, &intersection, scenery, prng);
        } else {
                out = mliAtmosphere_query(
                        &config->atmosphere, ray.support, ray.direction);
        }
        return out;
}
