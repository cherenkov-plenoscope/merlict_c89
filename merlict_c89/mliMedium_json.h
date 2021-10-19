/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIMEDIUM_JSON_H_
#define MERLICT_C89_MLIMEDIUM_JSON_H_

#include "mliMedium.h"
#include "mli_json.h"

int mliMedium_malloc_from_json_str(struct mliMedium *med, const char *json_str);
int mliMedium_malloc_from_json_token(
        struct mliMedium *med,
        const struct mliJson *json,
        const uint64_t token);
#endif
