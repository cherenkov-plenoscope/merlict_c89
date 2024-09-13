/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLITRACER_H_
#define MLITRACER_H_

#include <stdint.h>
#include "mliRay.h"
#include "mliColor.h"
#include "mliColorMaterials.h"
#include "mliAtmosphere.h"

struct mliScenery;
struct mliPrng;

struct mliTracerConfig {
        uint64_t num_trails_global_light_source;

        int have_atmosphere;
        struct mliAtmosphere atmosphere;

        struct mliColor background_color;
};

struct mliTracerConfig mliTracerConfig_init(void);

struct mliTracer {
        const struct mliScenery *scenery;
        const struct mliColorMaterials *scenery_color_materials;
        const struct mliTracerConfig *config;
};

struct mliTracer mliTracer_init(void);

struct mliColor mliTracer_trace_ray(
        const struct mliTracer *tracer,
        const struct mliRay ray,
        struct mliPrng *prng);

struct mliColor mliTracer_trace_ray_with_atmosphere(
        const struct mliTracer *tracer,
        const struct mliRay ray,
        struct mliPrng *prng);

struct mliColor mliTracer_trace_ray_without_atmosphere(
        const struct mliTracer *tracer,
        const struct mliRay ray,
        struct mliPrng *prng);

double mli_trace_sun_obstruction(
        const struct mliTracer *tracer,
        const struct mliVec position,
        struct mliPrng *prng);

double mli_trace_sun_visibility(
        const struct mliTracer *tracer,
        const struct mliVec position,
        struct mliPrng *prng);

#endif
