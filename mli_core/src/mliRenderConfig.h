/* Copyright 2018-2021 Sebastian Achim Mueller */
#ifndef MLIRENDERCONFIG_H_
#define MLIRENDERCONFIG_H_

#include <stdint.h>
#include "mliApertureCamera.h"
#include "mliHomTra.h"
#include "mliTracer.h"
#include "mli_json.h"

struct mliRenderConfig {
        struct mliApertureCamera camera;
        struct mliHomTraComp camera_to_root;
        struct mliTracerConfig tracer;
        uint64_t num_pixel_x;
        uint64_t num_pixel_y;
        uint64_t random_seed;
};

struct mliRenderConfig mliRenderConfig_init(void);

int mliRenderConfig_from_json_token(
        struct mliRenderConfig *cc,
        const struct mliJson *json,
        const uint64_t token);

#endif
