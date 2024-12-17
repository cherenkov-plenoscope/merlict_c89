/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "medium_json.h"
#include "../func/func_json.h"
#include "../chk/chk.h"

int mli_Medium_malloc_from_json_string(
        struct mli_Medium *med,
        const struct mli_String *str)
{
        struct mli_Json json = mli_Json_init();
        chk_msg(mli_Json_from_string(&json, str),
                "Failed to read json string to malloc medium.");
        chk_msg(mli_Medium_malloc_from_json_token(med, &json, 0),
                "Failed to malloc medium from json.");
        mli_Json_free(&json);
        return 1;
chk_error:
        return 0;
}

int mli_Medium_malloc_from_json_token(
        struct mli_Medium *med,
        const struct mli_Json *json,
        const uint64_t token)
{
        uint64_t refraction_token;
        uint64_t absorbtion_token;
        chk_msg(mli_Json_token_by_key(
                        json, token, "refraction", &refraction_token),
                "Expected medium to have key 'refraction', but it does not.");
        chk_msg(mli_Func_malloc_from_json_token(
                        &med->refraction, json, refraction_token + 1),
                "Failed to read medium's refraction from json.");
        chk_msg(mli_Json_token_by_key(
                        json, token, "absorbtion", &absorbtion_token),
                "Expected medium to have key 'absorbtion', but it does not.");
        chk_msg(mli_Func_malloc_from_json_token(
                        &med->absorbtion, json, absorbtion_token + 1),
                "Failed to read medium's absorbtion from json.");
        return 1;
chk_error:
        return 0;
}
