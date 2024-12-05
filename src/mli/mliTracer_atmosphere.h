/* Copyright 2018-2023 Sebastian Achim Mueller */
#ifndef MLITRACER_ATMOSPHERE_H_
#define MLITRACER_ATMOSPHERE_H_

#include "mliVec.h"
#include "mliScenery.h"
#include "mliColor.h"
#include "mliRay.h"
#include "mliTracer.h"
#include "mliIntersectionSurfaceNormal.h"

struct mliVec mli_random_direction_in_hemisphere(
        struct mli_Prng *prng,
        struct mliVec normal);
struct mliColor mli_trace_color_tone_of_sun(
        const struct mliTracerConfig *config,
        const struct mliVec support);
struct mliColor mli_trace_color_tone_of_diffuse_sky(
        const struct mliTracer *tracer,
        const struct mliIntersectionSurfaceNormal *intersection,
        struct mli_Prng *prng);
struct mliColor mli_trace_to_intersection_atmosphere(
        const struct mliTracer *tracer,
        const struct mliIntersectionSurfaceNormal *intersection,
        struct mli_Prng *prng);
struct mliColor mli_trace_with_atmosphere(
        const struct mliTracer *tracer,
        const struct mliRay ray,
        struct mli_Prng *prng);

#endif
