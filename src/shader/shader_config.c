/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "shader_config.h"

struct mli_shader_Config mli_shader_Config_init(void)
{
        struct mli_shader_Config config;
        config.background_color = mli_Color_set(128.0, 128.0, 128.0);
        config.num_trails_global_light_source = 3;
        config.have_atmosphere = 0;
        config.atmosphere = mli_Atmosphere_init();
        return config;
}
