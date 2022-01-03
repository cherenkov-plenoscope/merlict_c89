/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLITRACERCONFIG_JSON_H_
#define MLITRACERCONFIG_JSON_H_

#include "mliAtmosphere.h"
#include "mli_json.h"

int mliTracerConfig_from_json_token(
        struct mliTracerConfig *tc,
        const struct mliJson *json,
        const uint64_t tkn);
#endif
