/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "map_json.h"
#include "../chk/chk.h"

int mli_Map_insert_key_from_json(
        struct mli_Map *map,
        const struct mli_Json *json,
        const uint64_t token,
        const uint64_t value)
{
        struct mli_String buff = mli_String_init();
        const uint64_t name_strlen =
                (json->tokens[token].end - json->tokens[token].start);
        chk_msg(json->tokens[token].type == JSMN_STRING,
                "Expected key to be of type string.");
        chk_msg(mli_String_malloc(&buff, name_strlen),
                "Can not malloc String.");
        buff.size = buff.capacity;
        chk_msg(mli_Json_cstr_by_token(
                        json, token, buff.array, buff.capacity + 1),
                "Failed to extract string from json.");
        chk_msg(mli_Map_insert(map, &buff, value),
                "Failed to insert name and value into map.");
        mli_String_free(&buff);
        return 1;
chk_error:
        mli_String_free(&buff);
        mli_Json_debug_token_fprint(stderr, json, token);
        return 0;
}

int mli_Map_get_value_for_string_from_json(
        const struct mli_Map *map,
        const struct mli_Json *json,
        const uint64_t token,
        uint32_t *out_value)
{
        struct mli_String buff = mli_String_init();
        uint64_t value;
        uint64_t name_strlen =
                (json->tokens[token].end - json->tokens[token].start);
        chk_msg(json->tokens[token].type == JSMN_STRING,
                "Expected token to be of type string to be given to mliMap.");
        chk_msg(mli_String_malloc(&buff, name_strlen),
                "Can not malloc String.");
        chk_msg(mli_Json_cstr_by_token(
                        json, token, buff.array, buff.capacity + 1),
                "Failed to extract string from json.");
        buff.size = buff.capacity;
        chk_msg(mli_Map_get(map, &buff, &value),
                "Failed to get value for json-string-key from map.");
        (*out_value) = (uint32_t)value;

        mli_String_free(&buff);
        return 1;
chk_error:
        mli_String_free(&buff);
        mli_Json_debug_token_fprint(stderr, json, token);
        return 0;
}
