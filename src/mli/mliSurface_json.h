/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLISURFACE_JSON_H_
#define MLISURFACE_JSON_H_

#include <stdint.h>
#include "mliSurface.h"
#include "../json/json.h"

int mliSurface_malloc_from_json_str(
        struct mliSurface *surface,
        const char *json_str);
int mliSurface_malloc_from_json_token(
        struct mliSurface *surface,
        const struct mli_Json *json,
        const uint64_t token);
int mli_material_type_from_json_token(
        const struct mli_Json *json,
        const uint64_t token,
        uint32_t *material);
#endif
