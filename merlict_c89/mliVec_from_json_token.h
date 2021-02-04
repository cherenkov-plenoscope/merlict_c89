/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIVEC_FROM_JSON_TOKEN_H_
#define MERLICT_C89_MLIVEC_FROM_JSON_TOKEN_H_

#include "mliVec.h"
#include "mliColor.h"
#include "mli_json.h"

int mliColor_from_json_token(
        struct mliColor *c,
        const struct mliJson *json,
        const uint64_t token);
int mliVec_from_json_token(
        struct mliVec *v,
        const struct mliJson *json,
        const uint64_t token);
#endif
