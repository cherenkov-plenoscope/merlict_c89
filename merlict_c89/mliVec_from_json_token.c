/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliVec_from_json_token.h"

/* Copyright 2019 Sebastian Achim Mueller */

int mliVec_from_json_token(
        struct mliVec *v,
        const struct mliJson *json,
        const uint64_t token)
{
        mli_check(
                json->tokens[token].type == JSMN_ARRAY,
                "Expected vec-token to be a json-array.");
        mli_check(
                json->tokens[token].size == 3,
                "Expected vec-token to contain exactly 3 tokens.");
        mli_check(
                mliJson_as_float64(json, token + 1, &v->x),
                "Can not parse mliVec-x-value.");
        mli_check(
                mliJson_as_float64(json, token + 2, &v->y),
                "Can not parse mliVec y-value.");
        mli_check(
                mliJson_as_float64(json, token + 3, &v->z),
                "Can not parse mliVec z-value.");
        return 1;
error:
        return 0;
}

int mliColor_from_json_token(
        struct mliColor *c,
        const struct mliJson *json,
        const uint64_t token)
{
        struct mliVec v;
        mli_check(
                mliVec_from_json_token(&v, json, token),
                "Can not parse json-float-triple to color.");
        c->r = v.x;
        c->g = v.y;
        c->b = v.z;
        return 1;
error:
        return 0;
}
