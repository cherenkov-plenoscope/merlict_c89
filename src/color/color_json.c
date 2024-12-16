/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "color_json.h"
#include "../vec/vec_json.h"
#include "../chk/chk.h"

int mli_Color_from_json_token(
        struct mli_Color *c,
        const struct mli_Json *json,
        const uint64_t token)
{
        struct mli_Vec v;
        chk_msg(mli_Vec_from_json_token(&v, json, token),
                "Can not parse json-float-triple to color.");
        c->r = v.x;
        c->g = v.y;
        c->b = v.z;
        return 1;
chk_error:
        return 0;
}
