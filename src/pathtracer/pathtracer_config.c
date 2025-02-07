/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "pathtracer_config.h"

struct mli_pathtracer_Config mli_pathtracer_Config_init(void)
{
        const double power_fraction_of_diffuse_sky_relative_to_direct_sun =
                1e-1;
        struct mli_pathtracer_Config config;
        mli_ColorSpectrum_set_radiance_of_black_body_W_per_m2_per_sr(
                &config.ambient_radiance_W_per_m2_per_sr, 5000.0);

        config.ambient_radiance_W_per_m2_per_sr =
                mli_ColorSpectrum_multiply_scalar(
                        config.ambient_radiance_W_per_m2_per_sr,
                        power_fraction_of_diffuse_sky_relative_to_direct_sun);

        config.num_trails_global_light_source = 3;
        config.have_atmosphere = 0;
        config.atmosphere = mli_Atmosphere_init();
        return config;
}
