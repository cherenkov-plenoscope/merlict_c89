/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliTracer.h"
#include <math.h>
#include <stdint.h>
#include "mliTracer_atmosphere.h"
#include "mli_intersection_and_scenery.h"
#include "mli_ray_octree_traversal.h"
#include "mli_random.h"
#include "mliIntersection.h"
#include "mli_ray_scenery_query.h"
#include "../chk/chk.h"

struct mliTracer mliTracer_init(void)
{
        struct mliTracer tracer;
        tracer.scenery = NULL;
        tracer.scenery_color_materials = NULL;
        tracer.config = NULL;
        return tracer;
}

double mli_trace_sun_visibility(
        const struct mliTracer *tracer,
        const struct mliVec position,
        struct mli_Prng *prng)
{
        return (1.0 - mli_trace_sun_obstruction(tracer, position, prng));
}

double mli_trace_sun_obstruction(
        const struct mliTracer *tracer,
        const struct mliVec position,
        struct mli_Prng *prng)
{
        uint64_t i;
        double num_obstructions = 0.0;

        for (i = 0; i < tracer->config->num_trails_global_light_source; i++) {
                struct mliVec pos_in_source = mliVec_add(
                        mliVec_multiply(
                                tracer->config->atmosphere.sunDirection,
                                tracer->config->atmosphere.sunDistance),
                        mliVec_multiply(
                                mli_random_position_inside_unit_sphere(prng),
                                tracer->config->atmosphere.sunRadius));

                struct mliRay line_of_sight_to_source = mliRay_set(
                        position, mliVec_substract(pos_in_source, position));

                struct mliIntersection isec;

                const int has_intersection = mli_query_intersection(
                        tracer->scenery, line_of_sight_to_source, &isec);

                if (has_intersection) {
                        num_obstructions += 1.0;
                }
        }

        return num_obstructions /
               tracer->config->num_trails_global_light_source;
}

struct mli_Color mli_trace_to_intersection(
        const struct mliTracer *tracer,
        const struct mliIntersectionSurfaceNormal *intersection,
        struct mli_Prng *prng)
{
        struct mli_Color color;
        struct mliSide side;
        double theta;
        double lambert_factor;

        const double sun_visibility =
                mli_trace_sun_visibility(tracer, intersection->position, prng);

        side = mli_get_side_coming_from(tracer->scenery, intersection);
        color = tracer->scenery_color_materials->surfaces[side.surface]
                        .diffuse_reflection;

        theta = mliVec_angle_between(
                tracer->config->atmosphere.sunDirection,
                intersection->surface_normal);
        lambert_factor = fabs(cos(theta));

        color.r = color.r * 0.5 * (1.0 + sun_visibility * lambert_factor);
        color.g = color.g * 0.5 * (1.0 + sun_visibility * lambert_factor);
        color.b = color.b * 0.5 * (1.0 + sun_visibility * lambert_factor);

        return color;
}

struct mli_Color mliTracer_trace_ray_without_atmosphere(
        const struct mliTracer *tracer,
        const struct mliRay ray,
        struct mli_Prng *prng)
{
        struct mliIntersectionSurfaceNormal intersection =
                mliIntersectionSurfaceNormal_init();

        if (mli_query_intersection_with_surface_normal(
                    tracer->scenery, ray, &intersection)) {
                return mli_trace_to_intersection(tracer, &intersection, prng);
        } else {
                return tracer->config->background_color;
        }
}

struct mli_Color mliTracer_trace_ray(
        const struct mliTracer *tracer,
        const struct mliRay ray,
        struct mli_Prng *prng)
{
        if (tracer->config->have_atmosphere) {
                return mliTracer_trace_ray_with_atmosphere(tracer, ray, prng);
        } else {
                return mliTracer_trace_ray_without_atmosphere(
                        tracer, ray, prng);
        }
}

struct mliTracerConfig mliTracerConfig_init(void)
{
        struct mliTracerConfig config;
        config.background_color = mli_Color_set(128.0, 128.0, 128.0);
        config.num_trails_global_light_source = 3;
        config.have_atmosphere = 0;
        config.atmosphere = mliAtmosphere_init();
        return config;
}
