/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "func_json.h"
#include "../chk/chk.h"

int mli_Func_malloc_from_json_token(
        struct mli_Func *f,
        const struct mli_Json *json,
        const uint64_t token)
{
        uint64_t p;
        uint64_t point_token;
        chk_msg(json->tokens[token].type == JSMN_ARRAY,
                "Expected function-token to be a json-array.");
        chk_msg(mli_Func_malloc(f, json->tokens[token].size),
                "Can not allocate function.");
        point_token = token + 1;
        for (p = 0; p < f->num_points; p++) {
                chk_msg(json->tokens[point_token].type == JSMN_ARRAY,
                        "Expected function-x-y-pair to be a json-array.");
                chk_msg(json->tokens[point_token].size == 2,
                        "Expected function-x-y-pair to contain exactly 2 "
                        "tokens (x and y).");
                chk_msg(mli_Json_double_by_token(
                                json, point_token + 1, &f->x[p]),
                        "Can not parse function-x-value.");
                chk_msg(mli_Json_double_by_token(
                                json, point_token + 2, &f->y[p]),
                        "Can not parse function-y-value.");
                point_token += 3;
        }
        return 1;
chk_error:
        mli_Func_free(f);
        return 0;
}
