/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLICOLOR_JSON_H_
#define MLICOLOR_JSON_H_

#include <stdint.h>
#include "../color/color.h"
#include "../json/json.h"

int mli_Color_from_json_token(
        struct mli_Color *c,
        const struct mli_Json *json,
        const uint64_t token);
#endif
