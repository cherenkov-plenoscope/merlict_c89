/* Copyright 2018-2023 Sebastian Achim Mueller */
#ifndef MLI_SHADER_ATMOSPHERE_H_
#define MLI_SHADER_ATMOSPHERE_H_

#include "../vec/vec.h"
#include "../scenery/scenery.h"
#include "../color/color.h"
#include "../ray/ray.h"
#include "shader.h"
#include "../intersection/intersection_surface_normal.h"

struct mli_Color mli_trace_color_tone_of_sun(
        const struct mli_shader_Config *config,
        const struct mli_Vec support);
struct mli_Color mli_trace_color_tone_of_diffuse_sky(
        const struct mli_Shader *tracer,
        const struct mli_IntersectionSurfaceNormal *intersection,
        struct mli_Prng *prng);
struct mli_Color mli_trace_to_intersection_atmosphere(
        const struct mli_Shader *tracer,
        const struct mli_IntersectionSurfaceNormal *intersection,
        struct mli_Prng *prng);
struct mli_Color mli_trace_with_atmosphere(
        const struct mli_Shader *tracer,
        const struct mli_Ray ray,
        struct mli_Prng *prng);

#endif
