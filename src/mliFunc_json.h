/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIFUNC_JSON_H_
#define MLIFUNC_JSON_H_

#include <stdint.h>
#include "mliFunc.h"
#include "mli_json.h"

int mliFunc_malloc_from_json_token(
        struct mliFunc *func,
        const struct mliJson *json,
        const uint64_t token);
#endif
