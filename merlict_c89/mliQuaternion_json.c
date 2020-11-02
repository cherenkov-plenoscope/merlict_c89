/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliQuaternion_json.h"

int __mliQuaternion_tait_bryan_from_json(
        struct mliQuaternion *quat,
        const struct mliJson *json,
        const uint64_t token)
{
        uint64_t token_xyz;
        struct mliVec xyz;
        mli_check(
                mliJson_find_key(json, token, "xyz", &token_xyz),
                "Expected tait_bryan to have key 'xyz'.");
        mli_check(
                mliVec_from_json_token(&xyz, json, token_xyz + 1),
                "Failed to parse tait_bryan's 'xyz' from json.");
        *quat = mliQuaternion_set_tait_bryan(xyz.x, xyz.y, xyz.z);
        return 1;
error:
        return 0;
}

int __mliQuaternion_axis_angle_from_json(
        struct mliQuaternion *quat,
        const struct mliJson *json,
        const uint64_t token)
{
        uint64_t token_axis, token_angle;
        double angle;
        struct mliVec axis;
        mli_check(
                mliJson_find_key(json, token, "axis", &token_axis),
                "Expected axis_angle to have key 'axis'.");
        mli_check(
                mliVec_from_json_token(&axis, json, token_axis + 1),
                "Failed to parse axis_angle's 'axis' from json.");
        mli_check(
                mliJson_find_key(json, token, "angle", &token_angle),
                "Expected axis_angle to have key 'axis'.");
        mli_check(
                mliJson_as_float64(json, token_angle + 1, &angle),
                "Failed to parse axis_angle's 'angle' from json.");
        *quat = mliQuaternion_set_rotaxis_and_angle(axis, angle);
        return 1;
error:
        return 0;
}

int __mliQuaternion_quaternion_from_json(
        struct mliQuaternion *quat,
        const struct mliJson *json,
        const uint64_t token)
{
        uint64_t token_xyz;
        struct mliVec q;
        mli_check(
                mliJson_find_key(json, token, "xyz", &token_xyz),
                "Expected quaternion to have key 'xyz'.");
        mli_check(
                mliVec_from_json_token(&q, json, token_xyz + 1),
                "Failed to parse quaternion's 'xyz' from json.");
        *quat = mliQuaternion_set_unit_xyz(q.x, q.y, q.z);
        mli_check(
                fabs(mliQuaternion_norm(*quat) - 1.) < 1e-6,
                "Expected norm(quaternion) < 1e-6. Expected unit-quaternion.");
        return 1;
error:
        return 0;
}

int __mliQuaternion_from_json(
        struct mliQuaternion *quat,
        const struct mliJson *json,
        const uint64_t token)
{
        uint64_t token_repr = 0u;
        char *repr_str = NULL;
        uint64_t num_chars = 0u;
        mli_check(
                mliJson_find_key(json, token, "repr", &token_repr),
                "Expected 'rot' to have key 'repr'.");
        num_chars =
                (json->tokens[token_repr + 1].end -
                 json->tokens[token_repr + 1].start + 1u);
        mli_malloc(repr_str, char, num_chars);
        mliJson_as_string(json, token_repr + 1, repr_str, num_chars);
        if (strcmp(repr_str, "tait_bryan") == 0) {
                mli_check(
                        __mliQuaternion_tait_bryan_from_json(quat, json, token),
                        "Failed to parse tait_bryan rotation.");
        } else if (strcmp(repr_str, "axis_angle") == 0) {
                mli_check(
                        __mliQuaternion_axis_angle_from_json(quat, json, token),
                        "Failed to parse axis_angle rotation.");
        } else if (strcmp(repr_str, "quaternion") == 0) {
                mli_check(
                        __mliQuaternion_quaternion_from_json(quat, json, token),
                        "Failed to parse quaternion rotation.");
        } else {
                mli_sentinel("Unknown representation ('repr') in rotation.");
        }
        free(repr_str);
        return 1;
error:
        return 0;
}
