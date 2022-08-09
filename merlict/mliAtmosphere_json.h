/* Copyright 2018-2021 Sebastian Achim Mueller */
#ifndef MLIATMOSPHERE_JSON_H_
#define MLIATMOSPHERE_JSON_H_

#include "mliAtmosphere.h"
#include "mli_json.h"

int mliAtmosphere_from_json_token(
        struct mliAtmosphere *atm,
        const struct mliJson *json,
        const uint64_t tkn);

#endif
