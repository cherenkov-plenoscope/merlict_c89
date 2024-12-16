/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_VEC_JSON_H_
#define MLI_VEC_JSON_H_

#include <stdint.h>
#include "vec.h"
#include "../json/json.h"

int mli_Vec_from_json_token(
        struct mli_Vec *v,
        const struct mli_Json *json,
        const uint64_t token);
#endif
