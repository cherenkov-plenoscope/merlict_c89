/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mlivrConfig.h"

struct mlivrConfig mlivrConfig_default(void)
{
        struct mlivrConfig cfg;
        cfg.preview_num_cols = 160u;
        cfg.preview_num_rows = 90u / 2;

        cfg.export_num_cols = 1920u;
        cfg.export_num_rows = 1080u;

        cfg.step_length = 1.0;

        cfg.camera.position.x = 0.;
        cfg.camera.position.y = 0.;
        cfg.camera.position.z = 0.;

        cfg.camera.rotation.x = 0.;
        cfg.camera.rotation.y = -mli_deg2rad(90.);
        cfg.camera.rotation.z = 0.;

        cfg.camera.field_of_view = mli_deg2rad(80.);
        return cfg;
}
