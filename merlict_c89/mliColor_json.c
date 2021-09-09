/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliColor_json.h"
#include "mliVec_json.h"

int mliColor_from_json_token(
        struct mliColor *c,
        const struct mliJson *json,
        const uint64_t token)
{
        struct mliVec v;
        chk_msg(mliVec_from_json_token(&v, json, token),
                "Can not parse json-float-triple to color.");
        c->r = v.x;
        c->g = v.y;
        c->b = v.z;
        return 1;
error:
        return 0;
}
