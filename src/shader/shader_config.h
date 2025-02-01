/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_SHADER_CONFIG_H_
#define MLI_SHADER_CONFIG_H_

#include <stdint.h>
#include "../color/color.h"
#include "../color/color_materials.h"
#include "../atmosphere/atmosphere.h"

struct mli_Scenery;
struct mli_Prng;

struct mli_shader_Config {
        uint64_t num_trails_global_light_source;

        int have_atmosphere;
        struct mli_Atmosphere atmosphere;

        struct mli_ColorSpectrum ambient_radiance_W_per_m2_per_sr;
};

struct mli_shader_Config mli_shader_Config_init(void);

#endif
