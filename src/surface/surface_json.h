/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_SURFACE_JSON_H_
#define MLI_SURFACE_JSON_H_

#include <stdint.h>
#include "../surface/surface.h"
#include "../json/json.h"
#include "../string/string.h"

int mli_Surface_malloc_from_json_string(
        struct mli_Surface *surface,
        const struct mli_String *str);
int mli_Surface_malloc_from_json_token(
        struct mli_Surface *surface,
        const struct mli_Json *json,
        const uint64_t token);
int mli_material_type_from_json_token(
        const struct mli_Json *json,
        const uint64_t token,
        uint32_t *material);
#endif
