/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLICOLOR_JSON_H_
#define MLICOLOR_JSON_H_

#include "mliColor.h"
#include "mli_json.h"

int mliColor_from_json_token(
        struct mliColor *c,
        const struct mliJson *json,
        const uint64_t token);
#endif
