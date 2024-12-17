/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_MEDIUM_JSON_H_
#define MLI_MEDIUM_JSON_H_

#include <stdint.h>
#include "medium.h"
#include "../json/json.h"

int mli_Medium_malloc_from_json_string(
        struct mli_Medium *med,
        const struct mli_String *str);
int mli_Medium_malloc_from_json_token(
        struct mli_Medium *med,
        const struct mli_Json *json,
        const uint64_t token);
#endif
