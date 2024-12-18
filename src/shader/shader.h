/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_SHADER_H_
#define MLI_SHADER_H_

#include <stdint.h>
#include "../ray/ray.h"
#include "../color/color.h"
#include "../color/color_materials.h"
#include "../atmosphere/atmosphere.h"

struct mli_Scenery;
struct mli_Prng;

struct mli_shader_Config {
        uint64_t num_trails_global_light_source;

        int have_atmosphere;
        struct mli_Atmosphere atmosphere;

        struct mli_Color background_color;
};

struct mli_shader_Config mli_shader_Config_init(void);

struct mli_Shader {
        const struct mli_Scenery *scenery;
        const struct mli_ColorMaterials *scenery_color_materials;
        const struct mli_shader_Config *config;
};

struct mli_Shader mli_Shader_init(void);

struct mli_Color mli_Shader_trace_ray(
        const struct mli_Shader *tracer,
        const struct mli_Ray ray,
        struct mli_Prng *prng);

struct mli_Color mli_Shader_trace_ray_with_atmosphere(
        const struct mli_Shader *tracer,
        const struct mli_Ray ray,
        struct mli_Prng *prng);

struct mli_Color mli_Shader_trace_ray_without_atmosphere(
        const struct mli_Shader *tracer,
        const struct mli_Ray ray,
        struct mli_Prng *prng);

double mli_trace_sun_obstruction(
        const struct mli_Shader *tracer,
        const struct mli_Vec position,
        struct mli_Prng *prng);

double mli_trace_sun_visibility(
        const struct mli_Shader *tracer,
        const struct mli_Vec position,
        struct mli_Prng *prng);

#endif
