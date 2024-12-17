/* Copyright 2018-2021 Sebastian Achim Mueller */
#ifndef MLI_ATMOSPHERE_JSON_H_
#define MLI_ATMOSPHERE_JSON_H_

#include <stdint.h>
#include "atmosphere.h"
#include "../json/json.h"

int mli_Atmosphere_from_json_token(
        struct mli_Atmosphere *atm,
        const struct mli_Json *json,
        const uint64_t tkn);

#endif
