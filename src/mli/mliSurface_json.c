/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliSurface_json.h"
#include "mliName.h"
#include "../func/func_json.h"
#include "../color/color_json.h"
#include "../chk/chk.h"

int mliSurface_malloc_from_json_str(
        struct mliSurface *surface,
        const char *json_str)
{
        struct mli_Json json = mli_Json_init();
        chk_msg(mli_Json_malloc_from_cstr(&json, json_str),
                "Failed to read json_str to malloc surface.");
        chk_msg(mliSurface_malloc_from_json_token(surface, &json, 0),
                "Failed to malloc surface from json.");
        mli_Json_free(&json);
        return 1;
chk_error:
        return 0;
}

int mli_material_type_from_json_token(
        const struct mli_Json *json,
        const uint64_t token,
        uint32_t *material)
{
        struct mli_String buff = mli_String_init();
        const uint64_t name_strlen =
                (json->tokens[token + 1].end - json->tokens[token + 1].start);
        chk_msg(json->tokens[token + 1].type == JSMN_STRING,
                "Expected 'name' to be of type string.");
        chk(mli_String_malloc(&buff, name_strlen));
        buff.size = buff.capacity;
        chk_msg(mli_Json_cstr_by_token(
                        json, token + 1, buff.array, buff.size + 1),
                "Failed to extract string from json.");
        chk_msg(mli_material_type_from_string(&buff, material),
                "Failed to parse material type from json-string.");
        mli_String_free(&buff);
        return 1;
chk_error:
        mli_String_free(&buff);
        mli_Json_debug_token_fprint(stderr, json, token + 1);
        return 0;
}

int mliSurface_malloc_from_json_token(
        struct mliSurface *surface,
        const struct mli_Json *json,
        const uint64_t token)
{
        uint64_t token_specref;
        uint64_t token_diffref;
        uint64_t token_material;

        /* material */
        /* -------- */
        chk_msg(mli_Json_token_by_key(json, token, "material", &token_material),
                "Expected json-surface-item to contain key 'material'.");
        chk_msg(json->tokens[token_material].type == JSMN_STRING,
                "Expected surface's material to be of type string.");
        chk_msg(mli_material_type_from_json_token(
                        json, token_material, &surface->material),
                "Failed to get material-idx from map for string from json");

        /* specular_reflection */
        /* ------------------- */
        chk_msg(mli_Json_token_by_key(
                        json, token, "specular_reflection", &token_specref),
                "Expected surface to have key 'specular_reflection'.");
        chk_msg(mli_Func_malloc_from_json_token(
                        &surface->specular_reflection, json, token_specref + 1),
                "Failed to read surface's specular_reflection from json.");

        /* diffuse_reflection */
        /* ------------------ */
        chk_msg(mli_Json_token_by_key(
                        json, token, "diffuse_reflection", &token_diffref),
                "Expected surface to have key 'diffuse_reflection'.");
        chk_msg(mli_Func_malloc_from_json_token(
                        &surface->diffuse_reflection, json, token_diffref + 1),
                "Failed to read surface's diffuse_reflection from json.");

        return 1;
chk_error:
        return 0;
}
