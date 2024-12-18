/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_SHADER_CONFIG_H_
#define MLI_SHADER_CONFIG_H_

#include <stdint.h>
#include "../color/color.h"
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

#endif
