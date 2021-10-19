/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIDYNMAP_JSON_H_
#define MERLICT_C89_MLIDYNMAP_JSON_H_

#include <stdint.h>
#include "mli_json.h"
#include "mliDynMap.h"

int mliDynMap_get_value_for_string_from_json(
        const struct mliDynMap *map,
        const struct mliJson *json,
        const uint64_t token_name,
        uint32_t *out_value);
int mliDynMap_insert_key_from_json(
        struct mliDynMap *map,
        const struct mliJson *json,
        const uint64_t token_name,
        const uint64_t value);

#endif
