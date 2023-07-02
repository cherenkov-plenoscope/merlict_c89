/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLITRACER_H_
#define MLITRACER_H_

#include <stdint.h>
#include "mliRay.h"
#include "mli_random_generator.h"
#include "mliScenery.h"
#include "mliAtmosphere.h"

struct mliTracerConfig {
        uint64_t num_trails_global_light_source;

        int have_atmosphere;
        struct mliAtmosphere atmosphere;

        struct mliColor background_color;
};

struct mliTracerConfig mliTracerConfig_init(void);

struct mliColor mli_trace(
        const struct mliScenery *scenery,
        const struct mliRay ray,
        const struct mliTracerConfig *config,
        struct mliPrng *prng);

double mli_trace_sun_obstruction(
        const struct mliScenery *scenery,
        const struct mliVec position,
        const struct mliTracerConfig *config,
        struct mliPrng *prng);

double mli_trace_sun_visibility(
        const struct mliScenery *scenery,
        const struct mliVec position,
        const struct mliTracerConfig *config,
        struct mliPrng *prng);

#endif
