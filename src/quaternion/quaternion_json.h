/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_QUATERNION_JSON_H_
#define MLI_QUATERNION_JSON_H_

#include <stdint.h>
#include "quaternion.h"
#include "../chk/chk.h"
#include "../json/json.h"

chk_rc mli_Quaternion_tait_bryan_from_json(
        struct mli_Quaternion *quat,
        const struct mli_Json *json,
        const uint64_t token);
chk_rc mli_Quaternion_axis_angle_from_json(
        struct mli_Quaternion *quat,
        const struct mli_Json *json,
        const uint64_t token);
chk_rc mli_Quaternion_quaternion_from_json(
        struct mli_Quaternion *quat,
        const struct mli_Json *json,
        const uint64_t token);
chk_rc mli_Quaternion_from_json(
        struct mli_Quaternion *quat,
        const struct mli_Json *json,
        const uint64_t token);

#endif
