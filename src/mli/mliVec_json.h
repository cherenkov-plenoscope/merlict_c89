/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIVEC_JSON_H_
#define MLIVEC_JSON_H_

#include <stdint.h>
#include "mliVec.h"
#include "../json/json.h"

int mliVec_from_json_token(
        struct mliVec *v,
        const struct mli_Json *json,
        const uint64_t token);
#endif
