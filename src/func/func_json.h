/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_FUNC_JSON_H_
#define MLI_FUNC_JSON_H_

#include <stdint.h>
#include "../func/func.h"
#include "../json/json.h"

int mli_Func_malloc_from_json_token(
        struct mli_Func *func,
        const struct mli_Json *json,
        const uint64_t token);
#endif
