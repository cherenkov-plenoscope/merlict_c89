/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "shader.h"
#include <math.h>
#include <stdint.h>
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

double mli_Shader_trace_sun_visibility(
        const struct mli_Shader *tracer,
        const struct mli_Vec position,
        struct mli_Prng *prng)
{
        return (1.0 - mli_Shader_trace_sun_obstruction(tracer, position, prng));
}

double mli_Shader_trace_sun_obstruction(
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

struct mli_ColorSpectrum mli_raytracing_phong(
        const struct mli_Shader *tracer,
        const struct mli_IntersectionSurfaceNormal *intersection,
        const struct mli_IntersectionLayer *intersection_layer,
        struct mli_Prng *prng)
{
        struct mli_ColorSpectrum spectrum = mli_ColorSpectrum_init_zeros();
        if (tracer) {
                return spectrum;
        }
        if (intersection) {
                return spectrum;
        }
        if (intersection_layer) {
                return spectrum;
        }
        if (prng) {
                return spectrum;
        }
        /*
        struct mli_BoundaryLayer_Surface_Phong *phong = NULL;

        phong = racer->scenery->materials

        spectrum =
        tracer->scenery_color_materials->surfaces[ilayer.side_coming_from.surface]
                        .diffuse_reflection;

        theta = mli_Vec_angle_between(
                tracer->config->atmosphere.sunDirection,
                intersection->surface_normal);

        lambert_factor = fabs(cos(theta));

        color.r = color.r * 0.5 * (1.0 + sun_visibility * lambert_factor);
        color.g = color.g * 0.5 * (1.0 + sun_visibility * lambert_factor);
        color.b = color.b * 0.5 * (1.0 + sun_visibility * lambert_factor);
        */
        return spectrum;
}

struct mli_ColorSpectrum mli_raytracing_to_intersection(
        const struct mli_Shader *tracer,
        const struct mli_IntersectionSurfaceNormal *intersection,
        struct mli_Prng *prng)
{
        struct mli_ColorSpectrum spectrum;
        struct mli_IntersectionLayer intersection_layer;
        /*
        double theta;
        double lambert_factor;

        const double sun_visibility = mli_Shader_trace_sun_visibility(
                tracer, intersection->position, prng);
        */
        intersection_layer = mli_raytracing_get_intersection_layer(
                tracer->scenery, intersection);

        switch (intersection_layer.side_coming_from.surface->type) {
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_PHONG:
                spectrum = mli_raytracing_phong(
                        tracer, intersection, &intersection_layer, prng);
                break;
        default:
                chk_warning("surface type is not implemented.");
                spectrum = mli_ColorSpectrum_init_zeros();
        }

        return spectrum;
}

struct mli_ColorSpectrum mli_Shader_trace_ray_without_atmosphere(
        const struct mli_Shader *tracer,
        const struct mli_Ray ray,
        struct mli_Prng *prng)
{
        struct mli_IntersectionSurfaceNormal intersection =
                mli_IntersectionSurfaceNormal_init();

        if (mli_raytracing_query_intersection_with_surface_normal(
                    tracer->scenery, ray, &intersection)) {
                return mli_raytracing_to_intersection(
                        tracer, &intersection, prng);
        } else {
                return tracer->config->ambient_radiance_W_per_m2_per_sr;
        }
}

struct mli_Color mli_Shader_trace_ray(
        const struct mli_Shader *tracer,
        const struct mli_Ray ray,
        struct mli_Prng *prng)
{
        /*

        */
        struct mli_ColorSpectrum spectrum;
        struct mli_Vec xyz, rgb;

        if (tracer->config->have_atmosphere) {
                spectrum =
                        mli_Shader_trace_ray_with_atmosphere(tracer, ray, prng);
        } else {
                spectrum = mli_Shader_trace_ray_without_atmosphere(
                        tracer, ray, prng);
        }

        xyz = mli_ColorMaterials_ColorSpectrum_to_xyz(
                tracer->scenery_color_materials, &spectrum);

        rgb = mli_Mat_dot_product(
                &tracer->scenery_color_materials
                         ->observer_matching_curve_xyz_to_rgb,
                xyz);

        return mli_Color_set(rgb.x, rgb.y, rgb.z);
}
