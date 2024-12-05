/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLITRACERCONFIG_JSON_H_
#define MLITRACERCONFIG_JSON_H_

#include <stdint.h>
#include "mliTracer.h"
#include "../json/json.h"

int mliTracerConfig_from_json_token(
        struct mliTracerConfig *tc,
        const struct mli_Json *json,
        const uint64_t tkn);
#endif
