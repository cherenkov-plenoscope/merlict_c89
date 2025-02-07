/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "vec_json.h"

chk_rc mli_Vec_from_json_token(
        struct mli_Vec *v,
        const struct mli_Json *json,
        const uint64_t token)
{
        chk_msg(json->tokens[token].type == JSMN_ARRAY,
                "Expected vec-token to be a json-array.");
        chk_msg(json->tokens[token].size == 3,
                "Expected vec-token to contain exactly 3 tokens.");
        chk_msg(mli_Json_double_by_token(json, token + 1, &v->x),
                "Can not parse mli_Vec-x-value.");
        chk_msg(mli_Json_double_by_token(json, token + 2, &v->y),
                "Can not parse mli_Vec y-value.");
        chk_msg(mli_Json_double_by_token(json, token + 3, &v->z),
                "Can not parse mli_Vec z-value.");
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}
