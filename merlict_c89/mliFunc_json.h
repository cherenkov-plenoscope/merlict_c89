/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIFUNC_JSON_H_
#define MERLICT_C89_MLIFUNC_JSON_H_

#include "mliFunc.h"
#include "mliJson.h"

int mliFunc_malloc_from_json_token(
        struct mliFunc *func,
        const struct mliJson *json,
        const uint64_t token);
#endif
