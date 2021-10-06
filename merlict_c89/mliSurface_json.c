/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliSurface_json.h"

int mliSurface_malloc_from_json_str(
        struct mliSurface *surface,
        const char *json_str)
{
        struct mliJson json = mliJson_init();
        chk_msg(mliJson_malloc_from_string(&json, json_str),
                "Failed to read json_str to malloc surface.");
        chk_msg(mliSurface_malloc_from_json_token(surface, &json, 0),
                "Failed to malloc surface from json.");
        mliJson_free(&json);
        return 1;
error:
        return 0;
}

int _mli_material_type_from_json_token(
        const struct mliJson *json,
        const uint64_t token,
        uint32_t *material)
{
        char buff[MLI_NAME_CAPACITY] = {'\0'};
        const uint64_t name_strlen =
                (json->tokens[token + 1].end - json->tokens[token + 1].start);
        chk_msg(name_strlen < sizeof(buff), "Value of 'name' is too long");
        chk_msg(json->tokens[token + 1].type == JSMN_STRING,
                "Expected 'name' to be of type string.");
        chk_msg(mliJson_as_string(json, token + 1, buff, name_strlen + 1),
                "Failed to extract string from json.");
        chk_msg(mli_material_type_from_string(buff, material),
                "Failed to parse material type from json-string.");
        return 1;
error:
        mliJson_debug_token_fprint(stderr, json, token + 1);
        return 0;
}

int mliSurface_malloc_from_json_token(
        struct mliSurface *surface,
        const struct mliJson *json,
        const uint64_t token)
{
        uint64_t token_specref;
        uint64_t token_diffref;
        uint64_t token_color;
        uint64_t token_material;

        /* material */
        /* -------- */
        chk_msg(mliJson_find_key(json, token, "material", &token_material),
                "Expected json-surface-item to contain key 'material'.");
        chk_msg(json->tokens[token_material].type == JSMN_STRING,
                "Expected surface's material to be of type string.");
        chk_msg(_mli_material_type_from_json_token(
                        json, token_material, &surface->material),
                "Failed to get material-idx from map for string from json");

        /* specular_reflection */
        /* ------------------- */
        chk_msg(mliJson_find_key(
                        json, token, "specular_reflection", &token_specref),
                "Expected surface to have key 'specular_reflection'.");
        chk_msg(mliFunc_malloc_from_json_token(
                        &surface->specular_reflection, json, token_specref + 1),
                "Failed to read surface's specular_reflection from json.");

        /* diffuse_reflection */
        /* ------------------ */
        chk_msg(mliJson_find_key(
                        json, token, "diffuse_reflection", &token_diffref),
                "Expected surface to have key 'diffuse_reflection'.");
        chk_msg(mliFunc_malloc_from_json_token(
                        &surface->diffuse_reflection, json, token_diffref + 1),
                "Failed to read surface's diffuse_reflection from json.");

        /* color */
        /* ----- */
        chk_msg(mliJson_find_key(json, token, "color", &token_color),
                "Expected surface to have key 'color'.");
        chk_msg(mliColor_from_json_token(
                        &surface->color, json, token_color + 1),
                "Failed to read surface's color from json.");

        return 1;
error:
        return 0;
}
