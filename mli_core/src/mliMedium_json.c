/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliMedium_json.h"
#include "../../chk_debug/include/chk_debug.h"

int mliMedium_malloc_from_json_str(struct mliMedium *med, const char *json_str)
{
        struct mliJson json = mliJson_init();
        chk_msg(mliJson_malloc_from_cstr(&json, json_str),
                "Failed to read json_str to malloc medium.");
        chk_msg(mliMedium_malloc_from_json_token(med, &json, 0),
                "Failed to malloc medium from json.");
        mliJson_free(&json);
        return 1;
error:
        return 0;
}

int mliMedium_malloc_from_json_token(
        struct mliMedium *med,
        const struct mliJson *json,
        const uint64_t token)
{
        uint64_t refraction_token;
        uint64_t absorbtion_token;
        chk_msg(mliJson_token_by_key(
                        json, token, "refraction", &refraction_token),
                "Expected medium to have key 'refraction', but it does not.");
        chk_msg(mliFunc_malloc_from_json_token(
                        &med->refraction, json, refraction_token + 1),
                "Failed to read medium's refraction from json.");
        chk_msg(mliJson_token_by_key(
                        json, token, "absorbtion", &absorbtion_token),
                "Expected medium to have key 'absorbtion', but it does not.");
        chk_msg(mliFunc_malloc_from_json_token(
                        &med->absorbtion, json, absorbtion_token + 1),
                "Failed to read medium's absorbtion from json.");
        return 1;
error:
        return 0;
}
