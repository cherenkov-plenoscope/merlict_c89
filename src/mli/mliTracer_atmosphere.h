/* Copyright 2018-2023 Sebastian Achim Mueller */
#ifndef MLITRACER_ATMOSPHERE_H_
#define MLITRACER_ATMOSPHERE_H_

#include "../vec/vec.h"
#include "../scenery/scenery.h"
#include "../color/color.h"
#include "../ray/ray.h"
#include "mliTracer.h"
#include "mliIntersectionSurfaceNormal.h"

struct mli_Vec mli_random_direction_in_hemisphere(
        struct mli_Prng *prng,
        struct mli_Vec normal);
struct mli_Color mli_trace_color_tone_of_sun(
        const struct mliTracerConfig *config,
        const struct mli_Vec support);
struct mli_Color mli_trace_color_tone_of_diffuse_sky(
        const struct mliTracer *tracer,
        const struct mliIntersectionSurfaceNormal *intersection,
        struct mli_Prng *prng);
struct mli_Color mli_trace_to_intersection_atmosphere(
        const struct mliTracer *tracer,
        const struct mliIntersectionSurfaceNormal *intersection,
        struct mli_Prng *prng);
struct mli_Color mli_trace_with_atmosphere(
        const struct mliTracer *tracer,
        const struct mli_Ray ray,
        struct mli_Prng *prng);

#endif
