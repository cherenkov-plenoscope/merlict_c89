/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLICOLOR_JSON_H_
#define MERLICT_C89_MLICOLOR_JSON_H_

#include "mliColor.h"
#include "mliJson.h"

int mliColor_from_json_token(
        struct mliColor *c,
        const struct mliJson *json,
        const uint64_t token);
#endif
