/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "shader.h"
#include <math.h>
#include <stdint.h>
#include <assert.h>
#include "shader_atmosphere.h"
#include "../raytracing/intersection_and_scenery.h"
#include "../raytracing/ray_octree_traversal.h"
#include "../vec/vec_random.h"
#include "../fresnel/fresnel.h"
#include "../intersection/intersection.h"
#include "../raytracing/ray_scenery_query.h"
#include "../chk/chk.h"

struct mli_ShaderPath mli_ShaderPath_init(void)
{
        struct mli_ShaderPath out;
        out.weight = 1.0;
        out.num_interactions = 0u;
        return out;
}

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
        struct mli_ShaderPath path = mli_ShaderPath_init();

        spectrum = mli_Shader_trace_path_to_next_intersection(
                tracer, ray, path, prng);

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
        struct mli_ShaderPath path,
        struct mli_Prng *prng)
{
        struct mli_IntersectionSurfaceNormal intersection =
                mli_IntersectionSurfaceNormal_init();
        struct mli_ColorSpectrum out;
        int has_intersection = 0;

        if (path.weight < 0.05 || path.num_interactions > 25) {
                return mli_ColorSpectrum_init_zeros();
        }

        path.num_interactions += 1;

        has_intersection =
                mli_raytracing_query_intersection_with_surface_normal(
                        tracer->scenery, ray, &intersection);

        if (has_intersection) {
                out = mli_Shader_trace_next_intersection(
                        tracer, ray, &intersection, path, prng);
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
        const struct mli_Ray ray,
        const struct mli_IntersectionSurfaceNormal *intersection,
        struct mli_ShaderPath path,
        struct mli_Prng *prng)
{
        struct mli_ColorSpectrum out;
        struct mli_IntersectionLayer intersection_layer;

        intersection_layer = mli_raytracing_get_intersection_layer(
                tracer->scenery, intersection);

        switch (intersection_layer.side_coming_from.surface->type) {
        case MLI_SURFACE_TYPE_COOKTORRANCE:
                out = mli_Shader_trace_intersection_cooktorrance(
                        tracer,
                        ray,
                        intersection,
                        &intersection_layer,
                        path,
                        prng);
                break;
        case MLI_SURFACE_TYPE_TRANSPARENT:
                out = mli_Shader_trace_intersection_transparent(
                        tracer,
                        ray,
                        intersection,
                        &intersection_layer,
                        path,
                        prng);
                break;
        default:
                chk_warning("surface type is not implemented.");
                out = mli_ColorSpectrum_init_zeros();
        }
        return out;
}

struct mli_ColorSpectrum mli_Shader_trace_intersection_cooktorrance(
        const struct mli_Shader *tracer,
        const struct mli_Ray ray,
        const struct mli_IntersectionSurfaceNormal *intersection,
        const struct mli_IntersectionLayer *intersection_layer,
        struct mli_ShaderPath path,
        struct mli_Prng *prng)
{
        struct mli_ColorSpectrum incoming;
        struct mli_ColorSpectrum outgoing;
        struct mli_ColorSpectrum reflection;
        const struct mli_Surface_CookTorrance *cook =
                &intersection_layer->side_coming_from.surface->data
                         .cooktorrance;
        double theta_source;
        double theta_view;
        double lambert_factor_source;
        double lambert_factor_view;
        double factor;

        assert(intersection_layer->side_coming_from.surface->type ==
               MLI_SURFACE_TYPE_COOKTORRANCE);

        incoming = mli_Shader_trace_ambient_sun(tracer, intersection, prng);

        reflection = tracer->scenery_color_materials->spectra
                             .array[cook->reflection_spectrum];

        theta_source = mli_Vec_angle_between(
                tracer->config->atmosphere.sunDirection,
                intersection->surface_normal);

        theta_view = mli_Vec_angle_between(
                ray.direction, intersection->surface_normal);

        lambert_factor_source = fabs(cos(theta_source));
        lambert_factor_view = fabs(cos(theta_view));

        factor = lambert_factor_source * lambert_factor_view *
                 cook->diffuse_weight;

        outgoing = mli_ColorSpectrum_multiply(incoming, reflection);
        outgoing = mli_ColorSpectrum_multiply_scalar(outgoing, factor);
        return outgoing;
}

struct mli_ColorSpectrum mli_Shader_trace_intersection_transparent(
        const struct mli_Shader *tracer,
        const struct mli_Ray ray,
        const struct mli_IntersectionSurfaceNormal *intersection,
        const struct mli_IntersectionLayer *intersection_layer,
        struct mli_ShaderPath path,
        struct mli_Prng *prng)
{
        const uint64_t WAVELENGTH_BIN = 12;
        const uint64_t n_from_idx = intersection_layer->side_coming_from.medium
                                            ->refraction_spectrum;
        const uint64_t n_to_idx =
                intersection_layer->side_going_to.medium->refraction_spectrum;
        double n_from =
                tracer->scenery_color_materials->spectra.array[n_from_idx]
                        .values[WAVELENGTH_BIN];
        double n_to = tracer->scenery_color_materials->spectra.array[n_to_idx]
                              .values[WAVELENGTH_BIN];

        double reflection_weight = -1.0;
        double refraction_weight = -1.0;
        struct mli_Vec facing_surface_normal;
        struct mli_ColorSpectrum reflection_component;
        struct mli_ColorSpectrum refraction_component;
        struct mli_ColorSpectrum out = mli_ColorSpectrum_init_zeros();

        facing_surface_normal =
                intersection->from_outside_to_inside
                        ? intersection->surface_normal
                        : mli_Vec_multiply(intersection->surface_normal, -1.0);

        struct mli_Fresnel fresnel = mli_Fresnel_init(
                ray.direction, facing_surface_normal, n_from, n_to);

        reflection_weight = mli_Fresnel_reflection_propability(fresnel);
        refraction_weight = 1.0 - reflection_weight;

        assert(reflection_weight >= 0.0);
        assert(reflection_weight <= 1.0);

        assert(refraction_weight >= 0.0);
        assert(refraction_weight <= 1.0);

        if (path.weight * reflection_weight > 0.05) {
                struct mli_Ray nray = mli_Ray_set(
                        intersection->position,
                        mli_Fresnel_reflection_direction(fresnel));

                path.weight *= reflection_weight;
                reflection_component =
                        mli_Shader_trace_path_to_next_intersection(
                                tracer, nray, path, prng);

                reflection_component = mli_ColorSpectrum_multiply_scalar(
                        reflection_component, reflection_weight);

                out = mli_ColorSpectrum_add(out, reflection_component);
        }

        if (path.weight * refraction_weight > 0.05) {
                struct mli_Ray nray = mli_Ray_set(
                        intersection->position,
                        mli_Fresnel_refraction_direction(fresnel));
                path.weight *= refraction_weight;
                refraction_component =
                        mli_Shader_trace_path_to_next_intersection(
                                tracer, nray, path, prng);

                refraction_component = mli_ColorSpectrum_multiply_scalar(
                        refraction_component, refraction_weight);

                out = mli_ColorSpectrum_add(out, refraction_component);
        }

        return out;
}
