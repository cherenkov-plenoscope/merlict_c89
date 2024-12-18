/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliTracer.h"
#include <math.h>
#include <stdint.h>
#include "../vec/vec_random.h"
#include "../math/math.h"
#include "../intersection/intersection.h"
#include "../intersection/intersection_surface_normal.h"
#include "mli_ray_scenery_query.h"
#include "mli_intersection_and_scenery.h"

struct mli_Vec mli_random_direction_in_hemisphere(
        struct mli_Prng *prng,
        struct mli_Vec normal)
{
        struct mli_Vec rnd_dir;
        do {
                rnd_dir = mli_Vec_random_position_inside_unit_sphere(prng);
        } while (mli_Vec_dot(normal, rnd_dir) <= 0.0);
        return mli_Vec_normalized(rnd_dir);
}

struct mli_Color mli_trace_color_tone_of_sun(
        const struct mliTracerConfig *config,
        const struct mli_Vec support)
{
        struct mli_Color sun_color = mli_Color_set(1.0, 1.0, 1.0);
        double width_atmosphere = config->atmosphere.atmosphereRadius -
                                  config->atmosphere.earthRadius;

        if (config->atmosphere.altitude < width_atmosphere) {
                struct mli_Color color_close_to_sun = mli_Atmosphere_query(
                        &config->atmosphere,
                        support,
                        config->atmosphere.sunDirection);

                double f = config->atmosphere.sunDirection.z;
                double max = MLI_MATH_MAX3(
                        color_close_to_sun.r,
                        color_close_to_sun.g,
                        color_close_to_sun.b);

                color_close_to_sun.r /= max;
                color_close_to_sun.g /= max;
                color_close_to_sun.b /= max;

                return mli_Color_add(
                        mli_Color_multiply(sun_color, f),
                        mli_Color_multiply(color_close_to_sun, (1.0 - f)));
        } else {
                return sun_color;
        }
}

struct mli_Color mli_trace_color_tone_of_diffuse_sky(
        const struct mliTracer *tracer,
        const struct mliIntersectionSurfaceNormal *intersection,
        struct mli_Prng *prng)
{
        int i;
        struct mli_Color sky = mli_Color_set(0.0, 0.0, 0.0);
        struct mli_Ray obstruction_ray;
        struct mli_Vec facing_surface_normal;
        struct mliIntersection isec;
        int has_direct_view_to_sky = 0;
        int num_samples = 5;

        facing_surface_normal =
                intersection->from_outside_to_inside
                        ? intersection->surface_normal
                        : mli_Vec_multiply(intersection->surface_normal, -1.0);

        for (i = 0; i < num_samples; i++) {
                struct mli_Vec rnd_dir = mli_random_direction_in_hemisphere(
                        prng, facing_surface_normal);

                obstruction_ray.support = intersection->position;
                obstruction_ray.direction = rnd_dir;

                has_direct_view_to_sky = !mli_query_intersection(
                        tracer->scenery, obstruction_ray, &isec);

                if (has_direct_view_to_sky) {
                        struct mli_Color sample = mli_Atmosphere_query(
                                &tracer->config->atmosphere,
                                intersection->position,
                                rnd_dir);

                        double theta = mli_Vec_angle_between(
                                rnd_dir, facing_surface_normal);
                        double lambert_factor = fabs(cos(theta));

                        sky = mli_Color_add(
                                sky,
                                mli_Color_multiply(sample, lambert_factor));
                }
        }

        return mli_Color_multiply(sky, 1.0 / (255.0 * num_samples));
}

struct mli_Color mli_trace_to_intersection_atmosphere(
        const struct mliTracer *tracer,
        const struct mliIntersectionSurfaceNormal *intersection,
        struct mli_Prng *prng)
{
        struct mli_Color color;
        struct mli_Color tone;
        struct mli_boundarylayer_Side side;
        double theta;
        double lambert_factor;

        const double sun_visibility =
                mli_trace_sun_visibility(tracer, intersection->position, prng);

        if (sun_visibility > 0.0) {
                tone = mli_trace_color_tone_of_sun(
                        tracer->config, intersection->position);
                tone = mli_Color_multiply(tone, sun_visibility);
        } else {
                tone = mli_trace_color_tone_of_diffuse_sky(
                        tracer, intersection, prng);
        }

        side = mli_get_side_coming_from(tracer->scenery, intersection);
        color = tracer->scenery_color_materials->surfaces[side.surface]
                        .diffuse_reflection;

        theta = mli_Vec_angle_between(
                tracer->config->atmosphere.sunDirection,
                intersection->surface_normal);
        lambert_factor = fabs(cos(theta));

        color = mli_Color_multiply(color, lambert_factor);

        return mli_Color_multiply_elementwise(color, tone);
}

struct mli_Color mliTracer_trace_ray_with_atmosphere(
        const struct mliTracer *tracer,
        const struct mli_Ray ray,
        struct mli_Prng *prng)
{
        struct mliIntersectionSurfaceNormal intersection =
                mliIntersectionSurfaceNormal_init();
        struct mli_Color out;
        int has_intersection = mli_query_intersection_with_surface_normal(
                tracer->scenery, ray, &intersection);

        if (has_intersection) {
                out = mli_trace_to_intersection_atmosphere(
                        tracer, &intersection, prng);
        } else {
                out = mli_Atmosphere_query(
                        &tracer->config->atmosphere,
                        ray.support,
                        ray.direction);
        }
        return out;
}
