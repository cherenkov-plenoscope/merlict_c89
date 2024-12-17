/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIMEDIUM_JSON_H_
#define MLIMEDIUM_JSON_H_

#include <stdint.h>
#include "mliMedium.h"
#include "../json/json.h"

int mliMedium_malloc_from_json_string(
        struct mliMedium *med,
        const struct mli_String *str);
int mliMedium_malloc_from_json_token(
        struct mliMedium *med,
        const struct mli_Json *json,
        const uint64_t token);
#endif
