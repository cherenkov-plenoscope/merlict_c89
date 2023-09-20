/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliDynMap_json.h"
#include "chk_debug.h"

int mliDynMap_insert_key_from_json(
        struct mliDynMap *map,
        const struct mliJson *json,
        const uint64_t token,
        const uint64_t value)
{
        char buff[MLI_NAME_CAPACITY] = {'\0'};
        const uint64_t name_strlen =
                (json->tokens[token].end - json->tokens[token].start);
        chk_msg(name_strlen < sizeof(buff), "Key is too long");
        chk_msg(json->tokens[token].type == JSMN_STRING,
                "Expected key to be of type string.");
        chk_msg(mliJson_cstr_by_token(json, token, buff, name_strlen + 1),
                "Failed to extract string from json.");
        chk_msg(mliDynMap_insert(map, buff, value),
                "Failed to insert name and value into map.");
        return 1;
error:
        mliJson_debug_token_fprint(stderr, json, token);
        return 0;
}

int mliDynMap_get_value_for_string_from_json(
        const struct mliDynMap *map,
        const struct mliJson *json,
        const uint64_t token,
        uint32_t *out_value)
{
        char buff[MLI_NAME_CAPACITY] = {'\0'};
        uint64_t value;
        uint64_t name_strlen =
                (json->tokens[token].end - json->tokens[token].start);
        chk_msg(name_strlen < sizeof(buff), "Key is too long");
        chk_msg(json->tokens[token].type == JSMN_STRING,
                "Expected token to be of type string to be given to mliMap.");
        chk_msg(mliJson_cstr_by_token(json, token, buff, name_strlen + 1),
                "Failed to extract string from json.");
        chk_msg(mliDynMap_get(map, buff, &value),
                "Failed to get value for json-string-key from map.");
        (*out_value) = (uint32_t)value;

        return 1;
error:
        mliJson_debug_token_fprint(stderr, json, token);
        return 0;
}
