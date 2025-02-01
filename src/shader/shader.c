/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "shader.h"
#include <math.h>
#include <stdint.h>
#include <assert.h>
#include "shader_atmosphere.h"
#include "../raytracing/intersection_and_scenery.h"
#include "../raytracing/ray_octree_traversal.h"
#include "../vec/vec_random.h"
#include "../intersection/intersection.h"
#include "../raytracing/ray_scenery_query.h"
#include "../chk/chk.h"

struct mli_Shader mli_Shader_init(void)
{
        struct mli_Shader tracer;
        tracer.scenery = NULL;
        tracer.scenery_color_materials = NULL;
        tracer.config = NULL;
        return tracer;
}

double mli_Shader_estimate_sun_visibility_weight(
        const struct mli_Shader *tracer,
        const struct mli_Vec position,
        struct mli_Prng *prng)
{
        return (1.0 - mli_Shader_estimate_sun_obstruction_weight(
                              tracer, position, prng));
}

double mli_Shader_estimate_sun_obstruction_weight(
        const struct mli_Shader *tracer,
        const struct mli_Vec position,
        struct mli_Prng *prng)
{
        uint64_t i;
        double num_obstructions = 0.0;

        for (i = 0; i < tracer->config->num_trails_global_light_source; i++) {
                struct mli_Vec pos_in_source = mli_Vec_add(
                        mli_Vec_multiply(
                                tracer->config->atmosphere.sunDirection,
                                tracer->config->atmosphere.sunDistance),
                        mli_Vec_multiply(
                                mli_Vec_random_position_inside_unit_sphere(
                                        prng),
                                tracer->config->atmosphere.sunRadius));

                struct mli_Ray line_of_sight_to_source = mli_Ray_set(
                        position, mli_Vec_substract(pos_in_source, position));

                struct mli_Intersection isec;

                const int has_intersection = mli_raytracing_query_intersection(
                        tracer->scenery, line_of_sight_to_source, &isec);

                if (has_intersection) {
                        num_obstructions += 1.0;
                }
        }

        return num_obstructions /
               tracer->config->num_trails_global_light_source;
}

struct mli_Color mli_Shader_trace_ray(
        const struct mli_Shader *tracer,
        const struct mli_Ray ray,
        struct mli_Prng *prng)
{
        struct mli_ColorSpectrum spectrum;
        struct mli_Vec xyz, rgb;

        spectrum =
                mli_Shader_trace_path_to_next_intersection(tracer, ray, prng);

        xyz = mli_ColorMaterials_ColorSpectrum_to_xyz(
                tracer->scenery_color_materials, &spectrum);

        rgb = mli_Mat_dot_product(
                &tracer->scenery_color_materials
                         ->observer_matching_curve_xyz_to_rgb,
                xyz);

        return mli_Color_set(rgb.x, rgb.y, rgb.z);
}

struct mli_ColorSpectrum mli_Shader_trace_path_to_next_intersection(
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
                out = mli_Shader_trace_next_intersection(
                        tracer, &intersection, prng);
        } else {
                out = mli_Shader_trace_ambient_background(tracer, ray);
        }
        return out;
}

struct mli_ColorSpectrum mli_Shader_trace_ambient_background(
        const struct mli_Shader *tracer,
        const struct mli_Ray ray)
{
        if (tracer->config->have_atmosphere) {
                return mli_Shader_trace_ambient_background_atmosphere(
                        tracer, ray);
        } else {
                return mli_Shader_trace_ambient_background_whitebox(tracer);
        }
}

struct mli_ColorSpectrum mli_Shader_trace_ambient_background_atmosphere(
        const struct mli_Shader *tracer,
        const struct mli_Ray ray)
{
        return mli_Atmosphere_query(
                &tracer->config->atmosphere, ray.support, ray.direction);
}

struct mli_ColorSpectrum mli_Shader_trace_ambient_background_whitebox(
        const struct mli_Shader *tracer)
{
        return mli_ColorSpectrum_multiply_scalar(
                tracer->config->ambient_radiance_W_per_m2_per_sr, 0.5);
}

struct mli_ColorSpectrum mli_Shader_trace_ambient_sun(
        const struct mli_Shader *tracer,
        const struct mli_IntersectionSurfaceNormal *intersection,
        struct mli_Prng *prng)
{
        if (tracer->config->have_atmosphere) {
                return mli_Shader_trace_ambient_sun_atmosphere(
                        tracer, intersection, prng);
        } else {
                return mli_Shader_trace_ambient_sun_whitebox(
                        tracer, intersection, prng);
        }
}

struct mli_ColorSpectrum mli_Shader_trace_ambient_sun_atmosphere(
        const struct mli_Shader *tracer,
        const struct mli_IntersectionSurfaceNormal *intersection,
        struct mli_Prng *prng)
{
        struct mli_ColorSpectrum tone;

        const double sun_visibility = mli_Shader_estimate_sun_visibility_weight(
                tracer, intersection->position, prng);

        if (sun_visibility > 0.0) {
                tone = mli_raytracing_color_tone_of_sun(
                        tracer->config, intersection->position);
                tone = mli_ColorSpectrum_multiply_scalar(tone, sun_visibility);
        } else {
                tone = mli_raytracing_color_tone_of_diffuse_sky(
                        tracer, intersection, prng);
        }
        return tone;
}

struct mli_ColorSpectrum mli_Shader_trace_ambient_sun_whitebox(
        const struct mli_Shader *tracer,
        const struct mli_IntersectionSurfaceNormal *intersection,
        struct mli_Prng *prng)
{
        const double sun_visibility = mli_Shader_estimate_sun_visibility_weight(
                tracer, intersection->position, prng);

        return mli_ColorSpectrum_multiply_scalar(
                tracer->config->ambient_radiance_W_per_m2_per_sr,
                sun_visibility);
}

struct mli_ColorSpectrum mli_Shader_trace_next_intersection(
        const struct mli_Shader *tracer,
        const struct mli_IntersectionSurfaceNormal *intersection,
        struct mli_Prng *prng)
{
        struct mli_ColorSpectrum out;
        struct mli_IntersectionLayer intersection_layer;

        intersection_layer = mli_raytracing_get_intersection_layer(
                tracer->scenery, intersection);

        switch (intersection_layer.side_coming_from.surface->type) {
        case MLI_SURFACE_TYPE_COOKTORRANCE:
                out = mli_Shader_trace_intersection_cooktorrance(
                        tracer, intersection, &intersection_layer, prng);
                break;
        case MLI_SURFACE_TYPE_TRANSPARENT:
                out = mli_Shader_trace_intersection_transparent(
                        tracer, intersection, &intersection_layer, prng);
                break;
        default:
                chk_warning("surface type is not implemented.");
                out = mli_ColorSpectrum_init_zeros();
        }
        return out;
}

struct mli_ColorSpectrum mli_Shader_trace_intersection_cooktorrance(
        const struct mli_Shader *tracer,
        const struct mli_IntersectionSurfaceNormal *intersection,
        const struct mli_IntersectionLayer *intersection_layer,
        struct mli_Prng *prng)
{
        struct mli_ColorSpectrum incoming;
        struct mli_ColorSpectrum outgoing;
        struct mli_ColorSpectrum reflection;
        const struct mli_Surface_CookTorrance *cook =
                &intersection_layer->side_coming_from.surface->data
                         .cooktorrance;
        double theta;
        double lambert_factor;
        double factor;

        assert(intersection_layer->side_coming_from.surface->type ==
               MLI_SURFACE_TYPE_COOKTORRANCE);

        incoming = mli_Shader_trace_ambient_sun(tracer, intersection, prng);

        reflection = tracer->scenery_color_materials->spectra
                             .array[cook->reflection_spectrum];

        theta = mli_Vec_angle_between(
                tracer->config->atmosphere.sunDirection,
                intersection->surface_normal);

        lambert_factor = fabs(cos(theta));

        factor = lambert_factor * cook->diffuse_weight;

        outgoing = mli_ColorSpectrum_multiply(incoming, reflection);
        outgoing = mli_ColorSpectrum_multiply_scalar(outgoing, factor);
        return outgoing;
}

struct mli_ColorSpectrum mli_Shader_trace_intersection_transparent(
        const struct mli_Shader *tracer,
        const struct mli_IntersectionSurfaceNormal *intersection,
        const struct mli_IntersectionLayer *intersection_layer,
        struct mli_Prng *prng)
{
        const struct mli_Surface_Transparent *transparent =
                &intersection_layer->side_coming_from.surface->data.transparent;
        assert(intersection_layer->side_coming_from.surface->type ==
               MLI_SURFACE_TYPE_TRANSPARENT);

        return mli_ColorSpectrum_init_zeros();
}
