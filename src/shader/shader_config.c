/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "shader_config.h"

struct mli_shader_Config mli_shader_Config_init(void)
{
        struct mli_shader_Config config;
        mli_ColorSpectrum_set_radiance_of_black_body_W_per_m2_per_sr(
                &config.ambient_radiance_W_per_m2_per_sr, 5000.0);

        config.num_trails_global_light_source = 3;
        config.have_atmosphere = 0;
        config.atmosphere = mli_Atmosphere_init();
        return config;
}
