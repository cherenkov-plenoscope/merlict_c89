/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIQUATERNION_JSON_H_
#define MERLICT_C89_MLIQUATERNION_JSON_H_

#include "mliQuaternion.h"
#include "mliJson.h"

int _mliQuaternion_tait_bryan_from_json(
        struct mliQuaternion *quat,
        const struct mliJson *json,
        const uint64_t token);
int _mliQuaternion_axis_angle_from_json(
        struct mliQuaternion *quat,
        const struct mliJson *json,
        const uint64_t token);
int _mliQuaternion_quaternion_from_json(
        struct mliQuaternion *quat,
        const struct mliJson *json,
        const uint64_t token);
int mliQuaternion_from_json(
        struct mliQuaternion *quat,
        const struct mliJson *json,
        const uint64_t token);

#endif
