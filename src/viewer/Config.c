/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "Config.h"
#include "../math/math.h"

struct mli_viewer_Config mli_viewer_Config_default(void)
{
        struct mli_viewer_Config cfg;
        cfg.random_seed = 0u;

        cfg.preview_num_cols = 160u;
        cfg.preview_num_rows = 90u / 2;

        cfg.export_num_cols = 1280u;
        cfg.export_num_rows = 720u;

        cfg.step_length = 1.0;
        cfg.gain = 0.1;
        cfg.gamma = 1.0;

        cfg.view.position.x = 0.;
        cfg.view.position.y = 0.;
        cfg.view.position.z = 0.;

        cfg.view.rotation.x = mli_math_deg2rad(90.);
        cfg.view.rotation.y = 0.;
        cfg.view.rotation.z = 0.;

        cfg.view.field_of_view = mli_math_deg2rad(80.);

        cfg.aperture_camera_f_stop_ratio = 0.95;
        cfg.aperture_camera_image_sensor_width = 64e-3;
        return cfg;
}
