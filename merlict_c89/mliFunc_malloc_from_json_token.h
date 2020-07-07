/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIFUNC_MALLOC_FROM_JSON_TOKEN_H_
#define MERLICT_C89_MLIFUNC_MALLOC_FROM_JSON_TOKEN_H_

#include "mli_debug.h"
#include "mliFunc.h"
#include "mli_json.h"

int mliFunc_malloc_from_json_token(
        struct mliFunc *f,
        const struct mliJson *json,
        const uint64_t token);
#endif
