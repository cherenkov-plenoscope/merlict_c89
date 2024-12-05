/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLICOLOR_JSON_H_
#define MLICOLOR_JSON_H_

#include <stdint.h>
#include "mliColor.h"
#include "../json/json.h"

int mliColor_from_json_token(
        struct mliColor *c,
        const struct mli_Json *json,
        const uint64_t token);
#endif
