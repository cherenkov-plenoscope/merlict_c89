/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIVEC_JSON_H_
#define MERLICT_C89_MLIVEC_JSON_H_

#include "mliVec.h"
#include "mliJson.h"

int mliVec_from_json_token(
        struct mliVec *v,
        const struct mliJson *json,
        const uint64_t token);
#endif
