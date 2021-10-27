/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIQUATERNION_JSON_H_
#define MLIQUATERNION_JSON_H_

#include "mliQuaternion.h"
#include "mli_json.h"

int mliQuaternion_tait_bryan_from_json(
        struct mliQuaternion *quat,
        const struct mliJson *json,
        const uint64_t token);
int mliQuaternion_axis_angle_from_json(
        struct mliQuaternion *quat,
        const struct mliJson *json,
        const uint64_t token);
int mliQuaternion_quaternion_from_json(
        struct mliQuaternion *quat,
        const struct mliJson *json,
        const uint64_t token);
int mliQuaternion_from_json(
        struct mliQuaternion *quat,
        const struct mliJson *json,
        const uint64_t token);

#endif
