/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLITRACER_H_
#define MERLICT_C89_MLITRACER_H_

#include "mliScenery.h"

struct mliLightSource {
        struct mliVec position;
        double radius;
};

struct mliTracerCongig {
        struct mliColor background_color;

        uint64_t num_trails_global_light_source;
        struct mliLightSource global_light_source;
};

struct mliTracerCongig mliTracerCongig_init(void);

struct mliColor mli_trace(
        const struct mliScenery *scenery,
        const struct mliRay ray,
        const struct mliTracerCongig *config,
        struct mliMT19937 *prng);
#endif
