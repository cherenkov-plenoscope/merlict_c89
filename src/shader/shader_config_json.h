/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_SHADER_CONFIG_JSON_H_
#define MLI_SHADER_CONFIG_JSON_H_

#include <stdint.h>
#include "shader.h"
#include "../json/json.h"

int mliTracerConfig_from_json_token(
        struct mliTracerConfig *tc,
        const struct mli_Json *json,
        const uint64_t tkn);
#endif
