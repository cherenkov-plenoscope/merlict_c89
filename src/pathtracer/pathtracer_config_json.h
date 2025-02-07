/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_PATHTRACER_CONFIG_JSON_H_
#define MLI_PATHTRACER_CONFIG_JSON_H_

#include <stdint.h>
#include "../chk/chk.h"
#include "pathtracer_config.h"
#include "../json/json.h"

chk_rc mli_pathtracer_Config_from_json_token(
        struct mli_pathtracer_Config *tc,
        const struct mli_Json *json,
        const uint64_t tkn);
#endif
