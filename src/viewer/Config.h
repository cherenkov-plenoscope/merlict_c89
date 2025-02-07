/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_VIEWER_CONFIG_H_
#define MLI_VIEWER_CONFIG_H_

#include <stdint.h>
#include "../view/view.h"

struct mli_viewer_Config {
        uint32_t random_seed;
        uint64_t preview_num_cols;
        uint64_t preview_num_rows;
        uint64_t export_num_cols;
        uint64_t export_num_rows;
        double step_length;
        struct mli_View view;
        double gain;
        double gamma;

        double aperture_camera_f_stop_ratio;
        double aperture_camera_image_sensor_width;
};

struct mli_viewer_Config mli_viewer_Config_default(void);

#endif
