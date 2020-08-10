/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_VIWER_MLIVR_CONFIG_H_
#define MERLICT_C89_VIWER_MLIVR_CONFIG_H_

struct mlivrConfig {
        uint32_t random_seed;
        uint64_t preview_num_cols;
        uint64_t preview_num_rows;
        uint64_t export_num_cols;
        uint64_t export_num_rows;
        double step_length;
        struct mliCamera camera;
};

struct mlivrConfig mlivrConfig_default(void);

#endif
