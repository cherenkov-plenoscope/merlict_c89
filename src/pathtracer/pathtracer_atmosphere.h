/* Copyright 2018-2023 Sebastian Achim Mueller */
#ifndef MLI_PATHTRACER_ATMOSPHERE_H_
#define MLI_PATHTRACER_ATMOSPHERE_H_

#include "../vec/vec.h"
#include "../scenery/scenery.h"
#include "../color/color_materials.h"

struct mli_Prng;
struct mli_PathTracer;
struct mli_pathtracer_Config;
struct mli_IntersectionSurfaceNormal;

struct mli_ColorSpectrum mli_raytracing_color_tone_of_sun(
        const struct mli_pathtracer_Config *config,
        const struct mli_Vec support);
struct mli_ColorSpectrum mli_raytracing_color_tone_of_diffuse_sky(
        const struct mli_PathTracer *tracer,
        const struct mli_IntersectionSurfaceNormal *intersection,
        struct mli_Prng *prng);

#endif
