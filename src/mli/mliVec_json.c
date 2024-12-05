/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliVec_json.h"
#include "../chk/chk.h"

int mliVec_from_json_token(
        struct mliVec *v,
        const struct mliJson *json,
        const uint64_t token)
{
        chk_msg(json->tokens[token].type == JSMN_ARRAY,
                "Expected vec-token to be a json-array.");
        chk_msg(json->tokens[token].size == 3,
                "Expected vec-token to contain exactly 3 tokens.");
        chk_msg(mliJson_double_by_token(json, token + 1, &v->x),
                "Can not parse mliVec-x-value.");
        chk_msg(mliJson_double_by_token(json, token + 2, &v->y),
                "Can not parse mliVec y-value.");
        chk_msg(mliJson_double_by_token(json, token + 3, &v->z),
                "Can not parse mliVec z-value.");
        return 1;
chk_error:
        return 0;
}
