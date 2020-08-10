/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mlivrConfig.h"

struct mlivrConfig mlivrConfig_default(void)
{
        struct mlivrConfig cfg;
        cfg.random_seed = 0u;

        cfg.preview_num_cols = 160u;
        cfg.preview_num_rows = 90u / 2;

        cfg.export_num_cols = 512u;
        cfg.export_num_rows = 288u;

        cfg.step_length = 1.0;

        cfg.view.position.x = 0.;
        cfg.view.position.y = 0.;
        cfg.view.position.z = 0.;

        cfg.view.rotation.x = mli_deg2rad(90.);
        cfg.view.rotation.y = 0.;
        cfg.view.rotation.z = 0.;

        cfg.view.field_of_view = mli_deg2rad(80.);
        return cfg;
}
