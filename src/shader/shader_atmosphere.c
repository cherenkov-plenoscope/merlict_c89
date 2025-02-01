/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "shader_atmosphere.h"
#include <math.h>
#include <stdint.h>
#include "../vec/vec_random.h"
#include "../math/math.h"
#include "../intersection/intersection.h"
#include "../intersection/intersection_surface_normal.h"
#include "../raytracing/ray_scenery_query.h"
#include "../raytracing/intersection_and_scenery.h"

struct mli_ColorSpectrum mli_raytracing_color_tone_of_sun(
        const struct mli_shader_Config *config,
        const struct mli_Vec support)
{
        struct mli_ColorSpectrum sun_spectrum = config->atmosphere.sun_spectrum;
        double width_atmosphere = config->atmosphere.atmosphereRadius -
                                  config->atmosphere.earthRadius;

        if (config->atmosphere.altitude < width_atmosphere) {
                struct mli_ColorSpectrum color_close_to_sun =
                        mli_Atmosphere_query(
                                &config->atmosphere,
                                support,
                                config->atmosphere.sunDirection);

                double f = config->atmosphere.sunDirection.z;
                uint64_t argmax = 0;
                double vmax = 1.0;
                MLI_MATH_ARRAY_ARGMAX(
                        color_close_to_sun.values,
                        MLI_COLORSPECTRUM_SIZE,
                        argmax);
                vmax = color_close_to_sun.values[argmax];
                color_close_to_sun = mli_ColorSpectrum_multiply_scalar(
                        color_close_to_sun, (1.0 / vmax));

                return mli_ColorSpectrum_add(
                        mli_ColorSpectrum_multiply_scalar(sun_spectrum, f),
                        mli_ColorSpectrum_multiply_scalar(
                                color_close_to_sun, (1.0 - f)));
        } else {
                return sun_spectrum;
        }
}

struct mli_ColorSpectrum mli_raytracing_color_tone_of_diffuse_sky(
        const struct mli_Shader *tracer,
        const struct mli_IntersectionSurfaceNormal *intersection,
        struct mli_Prng *prng)
{
        int i;
        struct mli_ColorSpectrum sky = mli_ColorSpectrum_init_zeros();
        struct mli_Ray obstruction_ray;
        struct mli_Vec facing_surface_normal;
        struct mli_Intersection isec;
        int has_direct_view_to_sky = 0;
        int num_samples = 5;

        facing_surface_normal =
                intersection->from_outside_to_inside
                        ? intersection->surface_normal
                        : mli_Vec_multiply(intersection->surface_normal, -1.0);

        for (i = 0; i < num_samples; i++) {
                struct mli_Vec rnd_dir = mli_Vec_random_direction_in_hemisphere(
                        prng, facing_surface_normal);

                obstruction_ray.support = intersection->position;
                obstruction_ray.direction = rnd_dir;

                has_direct_view_to_sky = !mli_raytracing_query_intersection(
                        tracer->scenery, obstruction_ray, &isec);

                if (has_direct_view_to_sky) {
                        struct mli_ColorSpectrum sample = mli_Atmosphere_query(
                                &tracer->config->atmosphere,
                                intersection->position,
                                rnd_dir);

                        double theta = mli_Vec_angle_between(
                                rnd_dir, facing_surface_normal);
                        double lambert_factor = fabs(cos(theta));

                        sky = mli_ColorSpectrum_add(
                                sky,
                                mli_ColorSpectrum_multiply_scalar(
                                        sample, lambert_factor));
                }
        }

        return mli_ColorSpectrum_multiply_scalar(
                sky, 1.0 / (255.0 * num_samples));
}

struct mli_ColorSpectrum mli_raytracing_to_intersection_atmosphere(
        const struct mli_Shader *tracer,
        const struct mli_IntersectionSurfaceNormal *intersection,
        struct mli_Prng *prng)
{
        struct mli_ColorSpectrum spectrum;
        struct mli_ColorSpectrum tone;
        struct mli_IntersectionLayer intersection_layer;
        double theta;
        double lambert_factor;

        const double sun_visibility = mli_Shader_trace_sun_visibility(
                tracer, intersection->position, prng);

        if (sun_visibility > 0.0) {
                tone = mli_raytracing_color_tone_of_sun(
                        tracer->config, intersection->position);
                tone = mli_ColorSpectrum_multiply_scalar(tone, sun_visibility);
        } else {
                tone = mli_raytracing_color_tone_of_diffuse_sky(
                        tracer, intersection, prng);
        }

        intersection_layer = mli_raytracing_get_intersection_layer(
                tracer->scenery, intersection);

        switch (intersection_layer.side_coming_from.surface->type) {
        case MLI_SURFACE_TYPE_COOK_TORRANCE:
                spectrum = mli_ColorSpectrum_init_zeros();
                break;
        default:
                chk_warning("surface type is not implemented.");
                spectrum = mli_ColorSpectrum_init_zeros();
        }

        theta = mli_Vec_angle_between(
                tracer->config->atmosphere.sunDirection,
                intersection->surface_normal);
        lambert_factor = fabs(cos(theta));

        spectrum = mli_ColorSpectrum_multiply_scalar(spectrum, lambert_factor);

        return mli_ColorSpectrum_multiply(spectrum, tone);
}

struct mli_ColorSpectrum mli_Shader_trace_ray_with_atmosphere(
        const struct mli_Shader *tracer,
        const struct mli_Ray ray,
        struct mli_Prng *prng)
{
        struct mli_IntersectionSurfaceNormal intersection =
                mli_IntersectionSurfaceNormal_init();
        struct mli_ColorSpectrum out;
        int has_intersection =
                mli_raytracing_query_intersection_with_surface_normal(
                        tracer->scenery, ray, &intersection);

        if (has_intersection) {
                out = mli_raytracing_to_intersection_atmosphere(
                        tracer, &intersection, prng);
        } else {
                out = mli_Atmosphere_query(
                        &tracer->config->atmosphere,
                        ray.support,
                        ray.direction);
        }
        return out;
}
