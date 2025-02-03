/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_SHADER_H_
#define MLI_SHADER_H_

#include <stdint.h>
#include "../ray/ray.h"
#include "../color/color.h"
#include "../color/color_materials.h"
#include "../atmosphere/atmosphere.h"

struct mli_shader_Config;
struct mli_Scenery;
struct mli_Prng;
struct mli_IntersectionSurfaceNormal;
struct mli_IntersectionLayer;

struct mli_PathTracer {
        const struct mli_Scenery *scenery;
        const struct mli_ColorMaterials *scenery_color_materials;
        const struct mli_shader_Config *config;
};

struct mli_pathtracer_Path {
        double weight;
        uint64_t num_interactions;
};

struct mli_pathtracer_Path mli_pathtracer_Path_init(void);

struct mli_PathTracer mli_pathtracer_init(void);

double mli_pathtracer_estimate_sun_obstruction_weight(
        const struct mli_PathTracer *tracer,
        const struct mli_Vec position,
        struct mli_Prng *prng);

double mli_pathtracer_estimate_sun_visibility_weight(
        const struct mli_PathTracer *tracer,
        const struct mli_Vec position,
        struct mli_Prng *prng);

struct mli_Color mli_pathtracer_trace_ray(
        const struct mli_PathTracer *tracer,
        const struct mli_Ray ray,
        struct mli_Prng *prng);

struct mli_ColorSpectrum mli_pathtracer_trace_ambient_background(
        const struct mli_PathTracer *tracer,
        const struct mli_Ray ray);

struct mli_ColorSpectrum mli_pathtracer_trace_ambient_background_atmosphere(
        const struct mli_PathTracer *tracer,
        const struct mli_Ray ray);

struct mli_ColorSpectrum mli_pathtracer_trace_ambient_background_whitebox(
        const struct mli_PathTracer *tracer);

struct mli_ColorSpectrum mli_pathtracer_trace_ambient_sun(
        const struct mli_PathTracer *tracer,
        const struct mli_IntersectionSurfaceNormal *intersection,
        struct mli_Prng *prng);

struct mli_ColorSpectrum mli_pathtracer_trace_ambient_sun_atmosphere(
        const struct mli_PathTracer *tracer,
        const struct mli_IntersectionSurfaceNormal *intersection,
        struct mli_Prng *prng);

struct mli_ColorSpectrum mli_pathtracer_trace_ambient_sun_whitebox(
        const struct mli_PathTracer *tracer,
        const struct mli_IntersectionSurfaceNormal *intersection,
        struct mli_Prng *prng);

struct mli_ColorSpectrum mli_pathtracer_trace_path_to_next_intersection(
        const struct mli_PathTracer *tracer,
        const struct mli_Ray ray,
        struct mli_pathtracer_Path path,
        struct mli_Prng *prng);

struct mli_ColorSpectrum mli_pathtracer_trace_next_intersection(
        const struct mli_PathTracer *tracer,
        const struct mli_Ray ray,
        const struct mli_IntersectionSurfaceNormal *intersection,
        struct mli_pathtracer_Path path,
        struct mli_Prng *prng);

struct mli_ColorSpectrum mli_pathtracer_trace_intersection_cooktorrance(
        const struct mli_PathTracer *tracer,
        const struct mli_Ray ray,
        const struct mli_IntersectionSurfaceNormal *intersection,
        const struct mli_IntersectionLayer *intersection_layer,
        struct mli_pathtracer_Path path,
        struct mli_Prng *prng);

struct mli_ColorSpectrum mli_pathtracer_trace_intersection_transparent(
        const struct mli_PathTracer *tracer,
        const struct mli_Ray ray,
        const struct mli_IntersectionSurfaceNormal *intersection,
        const struct mli_IntersectionLayer *intersection_layer,
        struct mli_pathtracer_Path path,
        struct mli_Prng *prng);

#endif
