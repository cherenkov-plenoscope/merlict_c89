/* Copyright 2019 Sebastian Achim Mueller */
#ifndef MERLICT_MLIFUNC_FROM_JSON_H_
#define MERLICT_MLIFUNC_FROM_JSON_H_

#include "mli_debug.h"
#include "mliFunc.h"
#include "mli_json.h"

int mliFunc_malloc_from_json_token(
    struct mliFunc *f,
    const struct mliJson *json,
    const uint64_t token) {
    uint64_t point;
    uint64_t point_token;
    mli_check(
        json->tokens[token].type == JSMN_ARRAY,
        "Expected func-token to be a json-array.");
    f->num_points = json->tokens[token].size;
    mli_check(
        mliFunc_malloc(f),
        "Can not allocate function.");
    point_token = token + 1;
    for (point = 0; point < f->num_points; point++) {
        double val;
        mli_check(
            json->tokens[point_token].type == JSMN_ARRAY,
            "Expected function-point to be a json-array.");
        mli_check(
            json->tokens[point_token].size == 2,
            "Expected function-point to contain exactly 2 tokens (x and y).");
        mli_check(
            mliJson_as_float64(json, point_token + 1, &val),
            "Can not parse function-x-value.");
        f->x[point] = val;
        mli_check(
            mliJson_as_float64(json, point_token + 2, &val),
            "Can not parse function-y-value.");
        f->y[point] = val;
        point_token += 3;
    }
    return 1;
error:
    mliFunc_free(f);
    return 0;
}

#endif
