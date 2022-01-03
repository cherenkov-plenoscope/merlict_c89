/* Copyright 2018-2021 Sebastian Achim Mueller */
#ifndef MLICAMERACONTROL_H_
#define MLICAMERACONTROL_H_

#include "mliApertureCamera.h"
#include "mliHomTra.h"
#include "mliTracer.h"
#include "mli_json.h"

struct mliCameraControl {
        struct mliApertureCamera camera;
        struct mliHomTraComp camera_to_root;
        struct mliTracerCongig tracer_config;
        uint64_t num_pixel_x;
        uint64_t num_pixel_y;
        uint64_t random_seed;
};

struct mliCameraControl mliCameraControl_init(void);

int mliCameraControl_from_json_token(
        struct mliCameraControl *cc,
        const struct mliJson *json,
        const uint64_t token);

#endif
