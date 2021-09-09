/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliQuaternion_json.h"

int __mliQuaternion_tait_bryan_from_json(
        struct mliQuaternion *quat,
        const struct mliJson *json,
        const uint64_t token)
{
        uint64_t token_xyz;
        struct mliVec xyz_deg;
        mli_check_message(
                mliJson_find_key(json, token, "xyz_deg", &token_xyz),
                "Expected tait_bryan to have key 'xyz_deg'.");
        mli_check_message(
                mliVec_from_json_token(&xyz_deg, json, token_xyz + 1),
                "Failed to parse tait_bryan's 'xyz_deg' from json.");
        *quat = mliQuaternion_set_tait_bryan(
                mli_deg2rad(xyz_deg.x),
                mli_deg2rad(xyz_deg.y),
                mli_deg2rad(xyz_deg.z));
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
        double angle_deg;
        struct mliVec axis;
        mli_check_message(
                mliJson_find_key(json, token, "axis", &token_axis),
                "Expected axis_angle to have key 'axis'.");
        mli_check_message(
                mliVec_from_json_token(&axis, json, token_axis + 1),
                "Failed to parse axis_angle's 'axis' from json.");
        mli_check_message(
                mliJson_find_key(json, token, "angle_deg", &token_angle),
                "Expected axis_angle to have key 'angle_deg'.");
        mli_check_message(
                mliJson_as_float64(json, token_angle + 1, &angle_deg),
                "Failed to parse axis_angle's 'angle_deg' from json.");
        *quat = mliQuaternion_set_rotaxis_and_angle(
                axis, mli_deg2rad(angle_deg));
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
        mli_check_message(
                mliJson_find_key(json, token, "xyz", &token_xyz),
                "Expected quaternion to have key 'xyz'.");
        mli_check_message(
                mliVec_from_json_token(&q, json, token_xyz + 1),
                "Failed to parse quaternion's 'xyz' from json.");
        *quat = mliQuaternion_set_unit_xyz(q.x, q.y, q.z);
        mli_check_message(
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
        uint64_t token_repr_value = 0u;
        mli_check_message(
                mliJson_find_key(json, token, "repr", &token_repr),
                "Expected 'rot' to have key 'repr'.");
        token_repr_value = token_repr + 1;

        if (mliJson_strcmp(json, token_repr_value, "tait_bryan")) {
                mli_check_message(
                        __mliQuaternion_tait_bryan_from_json(quat, json, token),
                        "Failed to parse tait_bryan rotation.");
        } else if (mliJson_strcmp(json, token_repr_value, "axis_angle")) {
                mli_check_message(
                        __mliQuaternion_axis_angle_from_json(quat, json, token),
                        "Failed to parse axis_angle rotation.");
        } else if (mliJson_strcmp(json, token_repr_value, "quaternion")) {
                mli_check_message(
                        __mliQuaternion_quaternion_from_json(quat, json, token),
                        "Failed to parse quaternion rotation.");
        } else {
                mli_sentinel("Unknown representation 'repr' in rotation.");
        }
        return 1;
error:
        return 0;
}
