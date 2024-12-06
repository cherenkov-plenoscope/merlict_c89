/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLITRACER_H_
#define MLITRACER_H_

#include <stdint.h>
#include "mliRay.h"
#include "../color/color.h"
#include "mliColorMaterials.h"
#include "mliAtmosphere.h"

struct mliScenery;
struct mli_Prng;

#define MLI_TRC_SPHERE_SHAPE 1

struct mli_trc_SphereShape {
        double radius;
};

union mli_trc_LightSourceShapes {
        struct mli_trc_SphereShape sphere;
};

struct mli_trc_LightSource {
        struct mliVec position;
        struct mli_Color emission_spectrum;
        uint64_t shape;
        union mli_trc_LightSourceShapes light_source_shape;
};

struct mliTracerConfig {
        uint64_t num_trails_global_light_source;

        int have_atmosphere;
        struct mliAtmosphere atmosphere;

        struct mli_Color background_color;
};

struct mliTracerConfig mliTracerConfig_init(void);

struct mliTracer {
        const struct mliScenery *scenery;
        const struct mli_ColorMaterials *scenery_color_materials;
        const struct mliTracerConfig *config;
};

struct mliTracer mliTracer_init(void);

struct mli_Color mliTracer_trace_ray(
        const struct mliTracer *tracer,
        const struct mliRay ray,
        struct mli_Prng *prng);

struct mli_Color mliTracer_trace_ray_with_atmosphere(
        const struct mliTracer *tracer,
        const struct mliRay ray,
        struct mli_Prng *prng);

struct mli_Color mliTracer_trace_ray_without_atmosphere(
        const struct mliTracer *tracer,
        const struct mliRay ray,
        struct mli_Prng *prng);

double mli_trace_sun_obstruction(
        const struct mliTracer *tracer,
        const struct mliVec position,
        struct mli_Prng *prng);

double mli_trace_sun_visibility(
        const struct mliTracer *tracer,
        const struct mliVec position,
        struct mli_Prng *prng);

#endif
