/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "surface.h"
#include "../chk/chk.h"
#include "../magicid/magicid.h"
#include "../string/string_serialize.h"
#include "../json/json.h"
#include "../json/json_walk.h"

struct mli_Surface mli_Surface_init(void)
{
        struct mli_Surface out;
        out.name = mli_String_init();
        out.type = MLI_SURFACE_TYPE_NONE;
        return out;
}

void mli_Surface_free(struct mli_Surface *self)
{
        mli_String_free(&self->name);
        (*self) = mli_Surface_init();
}

int mli_Surface_equal(const struct mli_Surface *a, const struct mli_Surface *b)
{
        chk_msg(a->type == b->type, "Different types of surface models.");
        chk_msg(mli_String_equal(&a->name, &b->name),
                "Different names of surface models.");

        switch (a->type) {
        case MLI_SURFACE_TYPE_TRANSPARENT:
                chk_msg(mli_Surface_Transparent_equal(
                                &a->data.transparent, &b->data.transparent),
                        "'transparent' surfaces are not equal.");
                break;
        case MLI_SURFACE_TYPE_COOKTORRANCE:
                chk_msg(mli_Surface_CookTorrance_equal(
                                &a->data.cooktorrance, &b->data.cooktorrance),
                        "'cook-torrance' surfaces are not equal.");
                break;
        default:
                chk_badf(("surface-type-id '%lu' is unknown.", a->type));
        }

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

int mli_Surface_type_to_string(const uint64_t type, struct mli_String *s)
{
        switch (type) {
        case MLI_SURFACE_TYPE_TRANSPARENT:
                chk(mli_String_from_cstr(s, "transparent"));
                break;
        case MLI_SURFACE_TYPE_COOKTORRANCE:
                chk(mli_String_from_cstr(s, "cook-torrance"));
                break;
        default:
                chk_badf(("surface-type-id '%lu' is unknown.", type));
        }
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

int mli_Surface_type_from_string(const struct mli_String *s, uint64_t *id)
{
        if (mli_String_equal_cstr(s, "transparent")) {
                (*id) = MLI_SURFACE_TYPE_TRANSPARENT;
                return CHK_SUCCESS;
        } else if (mli_String_equal_cstr(s, "cook-torrance")) {
                (*id) = MLI_SURFACE_TYPE_COOKTORRANCE;
                return CHK_SUCCESS;
        } else {
                chk_badf(("surface-type-string '%s' is unknown.", s->array));
        }
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

int mli_Surface_to_io(const struct mli_Surface *self, struct mli_IO *f)
{
        struct mli_MagicId magic = mli_MagicId_init();
        chk(mli_MagicId_set(&magic, "mli_Surface"));
        chk_IO_write(&magic, sizeof(struct mli_MagicId), 1u, f);

        chk_msg(mli_String_to_io(&self->name, f),
                "Can't write surface.name to io.");
        chk_IO_write(&self->type, sizeof(uint64_t), 1u, f);

        switch (self->type) {
        case MLI_SURFACE_TYPE_TRANSPARENT:
                chk_msg(mli_Surface_Transparent_to_io(
                                &self->data.transparent, f),
                        "Can't write 'transparent' surface to io.");
                break;
        case MLI_SURFACE_TYPE_COOKTORRANCE:
                chk_msg(mli_Surface_CookTorrance_to_io(
                                &self->data.cooktorrance, f),
                        "Can't write 'cook-torrance' surface to io.");
                break;
        default:
                chk_badf(("surface-type-id '%lu' is unknown.", self->type));
        }
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

int mli_Surface_from_io(struct mli_Surface *self, struct mli_IO *f)
{
        struct mli_MagicId magic;
        chk_IO_read(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk(mli_MagicId_has_word(&magic, "mli_Surface"));
        mli_MagicId_warn_version(&magic);

        chk_msg(mli_String_from_io(&self->name, f),
                "Can't read surface.name from io.");
        chk_IO_read(&self->type, sizeof(uint64_t), 1u, f);

        switch (self->type) {
        case MLI_SURFACE_TYPE_TRANSPARENT:
                chk_msg(mli_Surface_Transparent_from_io(
                                &self->data.transparent, f),
                        "Can't read 'transparent' surface from io.");
                break;
        case MLI_SURFACE_TYPE_COOKTORRANCE:
                chk_msg(mli_Surface_CookTorrance_from_io(
                                &self->data.cooktorrance, f),
                        "Can't read 'cook-torrance' surface from io.");
                break;
        default:
                chk_badf(("surface-type-id '%lu' is unknown.", self->type));
        }
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

int mli_Surface_from_json_string_and_name(
        struct mli_Surface *self,
        const struct mli_Map *spectra_names,
        const struct mli_String *json_string,
        const struct mli_String *name)
{
        struct mli_Json json = mli_Json_init();
        struct mli_JsonWalk walk = mli_JsonWalk_init();
        struct mli_String key = mli_String_init();

        chk_msg(mli_Json_from_string(&json, json_string),
                "Can't parse surface from json string.");
        walk = mli_JsonWalk_set(&json);

        chk_msg(mli_JsonWalk_to_key(&walk, "type"),
                "Expected field 'type' in surface json string.");

        chk_msg(mli_JsonWalk_get_string(&walk, &key),
                "Expected field 'type' to hold a string.");

        chk_msgf(
                mli_Surface_type_from_string(&key, &self->type),
                ("Can't map surface 'type':'%s' from json string.", key.array));

        chk_msg(mli_String_copy(&self->name, name), "Can't copy surface name.");

        switch (self->type) {
        case MLI_SURFACE_TYPE_TRANSPARENT:
                chk_msg(mli_Surface_Transparent_from_json_string(
                                &self->data.transparent,
                                spectra_names,
                                json_string),
                        "Can't parse 'transparent' surface from json.");
                break;
        case MLI_SURFACE_TYPE_COOKTORRANCE:
                chk_msg(mli_Surface_CookTorrance_from_json_string(
                                &self->data.cooktorrance,
                                spectra_names,
                                json_string),
                        "Can't parse 'cook-torrance' surface from json.");
                break;
        default:
                chk_badf(("surface-type-id '%lu' is unknown.", self->type));
        }

        mli_String_free(&key);
        mli_Json_free(&json);
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

int mli_Surface_valid_wrt_materials(
        const struct mli_Surface *self,
        const struct mli_Materials *materials)
{
        chk_msg(mli_String_valid(&self->name, 1), "name is invalid.");

        switch (self->type) {
        case MLI_SURFACE_TYPE_TRANSPARENT:
                chk_msg(mli_Surface_Transparent_valid_wrt_materials(
                                &self->data.transparent, materials),
                        "'transparent' surfaces are not valid.");
                break;
        case MLI_SURFACE_TYPE_COOKTORRANCE:
                chk_msg(mli_Surface_CookTorrance_valid_wrt_materials(
                                &self->data.cooktorrance, materials),
                        "'cook-torrance' surfaces are not valid.");
                break;
        default:
                chk_badf(("surface-type-id '%lu' is unknown.", self->type));
        }

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}
