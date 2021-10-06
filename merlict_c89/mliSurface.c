/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliSurface.h"

struct mliSurface mliSurface_init(void)
{
        struct mliSurface surface;
        surface.material = MLI_MATERIAL_PHONG;
        surface.specular_reflection = mliFunc_init();
        surface.diffuse_reflection = mliFunc_init();
        surface.color = mliColor_set(0.0, 0.0, 0.0);
        return surface;
}

void mliSurface_free(struct mliSurface *surface)
{
        mliFunc_free(&surface->specular_reflection);
        mliFunc_free(&surface->diffuse_reflection);
        (*surface) = mliSurface_init();
}

int mliSurface_malloc(
        struct mliSurface *surface,
        const uint32_t num_points_specular_reflection,
        const uint32_t num_points_diffuse_reflection)
{
        mliSurface_free(surface);
        chk(mliFunc_malloc(
                &surface->specular_reflection, num_points_specular_reflection));
        chk(mliFunc_malloc(
                &surface->diffuse_reflection, num_points_diffuse_reflection));
        return 1;
error:
        return 0;
}

int mliSurface_equal(const struct mliSurface *a, const struct mliSurface *b)
{
        if (a->material != b->material)
                return 0;
        if (!mliFunc_equal(a->specular_reflection, b->specular_reflection))
                return 0;
        if (!mliFunc_equal(a->diffuse_reflection, b->diffuse_reflection))
                return 0;
        if (!mliColor_equal(a->color, b->color))
                return 0;
        return 1;
}

int mli_material_type_to_string(const uint32_t type, char *s)
{
        switch (type) {
        case MLI_MATERIAL_PHONG:
                sprintf(s, "Phong");
                break;
        case MLI_MATERIAL_TRANSPARENT:
                sprintf(s, "transparent");
                break;
        default:
                chk_bad("material-type-id is unknown.");
        }
        return 1;
error:
        return 0;
}

int mli_material_type_from_string(const char *s, uint32_t *id)
{
        if (0 == strcmp(s, "Phong")) {
                (*id) = MLI_MATERIAL_PHONG;
                return 1;
        } else if (0 == strcmp(s, "transparent")) {
                (*id) = MLI_MATERIAL_TRANSPARENT;
                return 1;
        } else {
                chk_bad("material-type-string is unknown.");
        }
        return 1;
error:
        return 0;
}

int mliSurface_fwrite(const struct mliSurface *srf, FILE *f)
{
        struct mliMagicId magic = mliMagicId_init();
        /* magic identifier */
        chk(mliMagicId_set(&magic, "mliMedium"));
        chk_fwrite(&magic, sizeof(struct mliMagicId), 1u, f);

        chk_fwrite(&srf->material, sizeof(uint32_t), 1u, f);
        chk(mliFunc_fwrite(&srf->specular_reflection, f));
        chk(mliFunc_fwrite(&srf->diffuse_reflection, f));
        chk_fwrite(&srf->color, sizeof(struct mliColor), 1u, f);

        return 1;
error:
        return 0;
}

int mliSurface_malloc_fread(struct mliSurface *srf, FILE *f)
{
        struct mliMagicId magic;
        /* magic identifier */
        chk_fread(&magic, sizeof(struct mliMagicId), 1u, f);
        chk(mliMagicId_has_word(&magic, "mliSurface"));
        mliMagicId_warn_version(&magic);

        chk_fread(&srf->material, sizeof(uint32_t), 1u, f);
        chk(mliFunc_malloc_fread(&srf->specular_reflection, f));
        chk(mliFunc_malloc_fread(&srf->diffuse_reflection, f));
        chk_fread(&srf->color, sizeof(struct mliColor), 1u, f);

        return 1;
error:
        return 0;
}

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
