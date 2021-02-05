/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIQUATERNION_JSON_H_
#define MERLICT_C89_MLIQUATERNION_JSON_H_

#include "mli_debug.h"
#include "mliQuaternion.h"
#include "mliJson.h"

int __mliQuaternion_tait_bryan_from_json(
        struct mliQuaternion *quat,
        const struct mliJson *json,
        const uint64_t token);
int __mliQuaternion_axis_angle_from_json(
        struct mliQuaternion *quat,
        const struct mliJson *json,
        const uint64_t token);
int __mliQuaternion_quaternion_from_json(
        struct mliQuaternion *quat,
        const struct mliJson *json,
        const uint64_t token);
int __mliQuaternion_from_json(
        struct mliQuaternion *quat,
        const struct mliJson *json,
        const uint64_t token);

#endif
