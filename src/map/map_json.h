/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_MAP_JSON_H_
#define MLI_MAP_JSON_H_

#include <stdint.h>
#include "../chk/chk.h"
#include "../json/json.h"
#include "map.h"

chk_rc mli_Map_get_value_for_string_from_json(
        const struct mli_Map *map,
        const struct mli_Json *json,
        const uint64_t token_name,
        uint32_t *out_value);
chk_rc mli_Map_insert_key_from_json(
        struct mli_Map *map,
        const struct mli_Json *json,
        const uint64_t token_name,
        const uint64_t value);

#endif
