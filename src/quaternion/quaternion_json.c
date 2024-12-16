/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "quaternion_json.h"
#include "../vec/vec_json.h"
#include "../math/math.h"
#include "../chk/chk.h"

int mli_Quaternion_tait_bryan_from_json(
        struct mli_Quaternion *quat,
        const struct mli_Json *json,
        const uint64_t token)
{
        uint64_t token_xyz;
        struct mli_Vec xyz_deg;
        chk_msg(mli_Json_token_by_key(json, token, "xyz_deg", &token_xyz),
                "Expected tait_bryan to have key 'xyz_deg'.");
        chk_msg(mli_Vec_from_json_token(&xyz_deg, json, token_xyz + 1),
                "Failed to parse tait_bryan's 'xyz_deg' from json.");
        *quat = mli_Quaternion_set_tait_bryan(
                mli_math_deg2rad(xyz_deg.x),
                mli_math_deg2rad(xyz_deg.y),
                mli_math_deg2rad(xyz_deg.z));
        return 1;
chk_error:
        return 0;
}

int mli_Quaternion_axis_angle_from_json(
        struct mli_Quaternion *quat,
        const struct mli_Json *json,
        const uint64_t token)
{
        uint64_t token_axis, token_angle;
        double angle_deg;
        struct mli_Vec axis;
        chk_msg(mli_Json_token_by_key(json, token, "axis", &token_axis),
                "Expected axis_angle to have key 'axis'.");
        chk_msg(mli_Vec_from_json_token(&axis, json, token_axis + 1),
                "Failed to parse axis_angle's 'axis' from json.");
        chk_msg(mli_Json_token_by_key(json, token, "angle_deg", &token_angle),
                "Expected axis_angle to have key 'angle_deg'.");
        chk_msg(mli_Json_double_by_token(json, token_angle + 1, &angle_deg),
                "Failed to parse axis_angle's 'angle_deg' from json.");
        *quat = mli_Quaternion_set_rotaxis_and_angle(
                axis, mli_math_deg2rad(angle_deg));
        return 1;
chk_error:
        return 0;
}

int mli_Quaternion_quaternion_from_json(
        struct mli_Quaternion *quat,
        const struct mli_Json *json,
        const uint64_t token)
{
        uint64_t token_xyz;
        struct mli_Vec q;
        chk_msg(mli_Json_token_by_key(json, token, "xyz", &token_xyz),
                "Expected quaternion to have key 'xyz'.");
        chk_msg(mli_Vec_from_json_token(&q, json, token_xyz + 1),
                "Failed to parse quaternion's 'xyz' from json.");
        *quat = mli_Quaternion_set_unit_xyz(q.x, q.y, q.z);
        chk_msg(fabs(mli_Quaternion_norm(*quat) - 1.) < 1e-6,
                "Expected norm(quaternion) < 1e-6. Expected unit-quaternion.");
        return 1;
chk_error:
        return 0;
}

int mli_Quaternion_from_json(
        struct mli_Quaternion *quat,
        const struct mli_Json *json,
        const uint64_t token)
{
        uint64_t token_repr = 0u;
        uint64_t token_repr_value = 0u;
        chk_msg(mli_Json_token_by_key(json, token, "repr", &token_repr),
                "Expected 'rot' to have key 'repr'.");
        token_repr_value = token_repr + 1;

        if (mli_Json_cstrcmp(json, token_repr_value, "tait_bryan")) {
                chk_msg(mli_Quaternion_tait_bryan_from_json(quat, json, token),
                        "Failed to parse tait_bryan rotation.");
        } else if (mli_Json_cstrcmp(json, token_repr_value, "axis_angle")) {
                chk_msg(mli_Quaternion_axis_angle_from_json(quat, json, token),
                        "Failed to parse axis_angle rotation.");
        } else if (mli_Json_cstrcmp(json, token_repr_value, "quaternion")) {
                chk_msg(mli_Quaternion_quaternion_from_json(quat, json, token),
                        "Failed to parse quaternion rotation.");
        } else {
                chk_bad("Unknown representation 'repr' in rotation.");
        }
        return 1;
chk_error:
        return 0;
}
