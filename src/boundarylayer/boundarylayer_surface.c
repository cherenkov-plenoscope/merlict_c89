/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "boundarylayer_surface.h"
#include "../chk/chk.h"

struct mli_BoundaryLayer_Surface mli_BoundaryLayer_Surface_init(void)
{
        struct mli_BoundaryLayer_Surface out;
        out.name = mli_String_init();
        out.type = MLI_BOUNDARYLAYER_SURFACE_TYPE_NONE;
        return out;
}

int mli_BoundaryLayer_Surface_equal(
        const struct mli_BoundaryLayer_Surface *a,
        const struct mli_BoundaryLayer_Surface *b)
{
        chk_msg(a->type == b->type, "Different types of surface models.");
        chk_msg(mli_String_equal(&a->name, &b->name),
                "Different names of surface models.");

        switch (a->type) {
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_PHONG:
                chk_msg(mli_BoundaryLayer_Surface_Phong_equal(
                                &a->data.phong, &b->data.phong),
                        "'phong' surfaces are not equal.");
                break;
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_TRANSPARENT:
                chk_msg(mli_BoundaryLayer_Surface_Transparent_equal(
                                &a->data.transparent, &b->data.transparent),
                        "'transparent' surfaces are not equal.");
                break;
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_LAMBERTIAN:
                chk_msg(mli_BoundaryLayer_Surface_Lambertian_equal(
                                &a->data.lambertian, &b->data.lambertian),
                        "'lambertian' surfaces are not equal.");
                break;
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_MIRROR:
                chk_msg(mli_BoundaryLayer_Surface_Mirror_equal(
                                &a->data.mirror, &b->data.mirror),
                        "'mirror' surfaces are not equal.");
                break;
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_COOK_TORRANCE:
                chk_msg(mli_BoundaryLayer_Surface_Cook_Torrance_equal(
                                &a->data.cook_torrance, &b->data.cook_torrance),
                        "'cook-torrance' surfaces are not equal.");
                break;
        default:
                chk_bad("surface-type-id is unknown.");
        }

        return 1;
chk_error:
        return 0;
}

int mli_BoundaryLayer_Surface_type_to_string(
        const uint64_t type,
        struct mli_String *s)
{
        switch (type) {
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_PHONG:
                chk(mli_String_from_cstr(s, "phong"));
                break;
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_TRANSPARENT:
                chk(mli_String_from_cstr(s, "transparent"));
                break;
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_LAMBERTIAN:
                chk(mli_String_from_cstr(s, "lambertian"));
                break;
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_MIRROR:
                chk(mli_String_from_cstr(s, "mirror"));
                break;
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_COOK_TORRANCE:
                chk(mli_String_from_cstr(s, "cook-torrance"));
                break;
        default:
                chk_bad("surface-type-id is unknown.");
        }
        return 1;
chk_error:
        return 0;
}

int mli_BoundaryLayer_Surface_type_from_string(
        const struct mli_String *s,
        uint64_t *id)
{
        if (mli_String_equal_cstr(s, "phong")) {
                (*id) = MLI_BOUNDARYLAYER_SURFACE_TYPE_PHONG;
                return 1;
        } else if (mli_String_equal_cstr(s, "transparent")) {
                (*id) = MLI_BOUNDARYLAYER_SURFACE_TYPE_TRANSPARENT;
                return 1;
        } else if (mli_String_equal_cstr(s, "lambertian")) {
                (*id) = MLI_BOUNDARYLAYER_SURFACE_TYPE_LAMBERTIAN;
                return 1;
        } else if (mli_String_equal_cstr(s, "mirror")) {
                (*id) = MLI_BOUNDARYLAYER_SURFACE_TYPE_MIRROR;
                return 1;
        } else if (mli_String_equal_cstr(s, "cook-torrance")) {
                (*id) = MLI_BOUNDARYLAYER_SURFACE_TYPE_COOK_TORRANCE;
                return 1;
        } else {
                chk_bad("surface-type-string is unknown.");
        }
        return 1;
chk_error:
        return 0;
}

int mli_BoundaryLayer_Surface_to_io(
        const struct mli_BoundaryLayer_Surface *self,
        struct mli_IO *f)
{
        struct mli_MagicId magic = mli_MagicId_init();
        chk(mli_MagicId_set(&magic, "mli_BoundaryLayer_Surface"));
        chk_IO_write(&magic, sizeof(struct mli_MagicId), 1u, f);

        chk_msg(mli_String_to_io(&self->name, f),
                "Can't write surface.name to io.");
        chk_IO_write(&self->type, sizeof(uint64_t), 1u, f);

        switch (self->type) {
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_PHONG:
                chk_msg(mli_BoundaryLayer_Surface_Phong_to_io(
                                &self->data.phong, f),
                        "Can't write 'phong' surface to io.");
                break;
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_TRANSPARENT:
                chk_msg(mli_BoundaryLayer_Surface_Transparent_to_io(
                                &self->data.transparent, f),
                        "Can't write 'transparent' surface to io.");
                break;
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_LAMBERTIAN:
                chk_msg(mli_BoundaryLayer_Surface_Lambertian_to_io(
                                &self->data.lambertian, f),
                        "Can't write 'lambertian' surface to io.");
                break;
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_MIRROR:
                chk_msg(mli_BoundaryLayer_Surface_Mirror_to_io(
                                &self->data.mirror, f),
                        "Can't write 'mirror' surface to io.");
                break;
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_COOK_TORRANCE:
                chk_msg(mli_BoundaryLayer_Surface_Cook_Torrance_to_io(
                                &self->data.cook_torrance, f),
                        "Can't write 'cook-torrance' surface to io.");
                break;
        default:
                chk_bad("surface-type-id is unknown.");
        }
        return 1;
chk_error:
        return 0;
}

int mli_BoundaryLayer_Surface_from_io(
        struct mli_BoundaryLayer_Surface *self,
        struct mli_IO *f)
{
        struct mli_MagicId magic;
        chk_IO_read(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk(mli_MagicId_has_word(&magic, "mli_BoundaryLayer_Surface"));
        mli_MagicId_warn_version(&magic);

        chk_msg(mli_String_from_io(&self->name, f),
                "Can't read surface.name from io.");
        chk_IO_read(&self->type, sizeof(uint64_t), 1u, f);

        switch (self->type) {
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_PHONG:
                chk_msg(mli_BoundaryLayer_Surface_Phong_from_io(
                                &self->data.phong, f),
                        "Can't read 'phong' surface from io.");
                break;
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_TRANSPARENT:
                chk_msg(mli_BoundaryLayer_Surface_Transparent_from_io(
                                &self->data.transparent, f),
                        "Can't read 'transparent' surface from io.");
                break;
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_LAMBERTIAN:
                chk_msg(mli_BoundaryLayer_Surface_Lambertian_from_io(
                                &self->data.lambertian, f),
                        "Can't read 'lambertian' surface from io.");
                break;
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_MIRROR:
                chk_msg(mli_BoundaryLayer_Surface_Mirror_from_io(
                                &self->data.mirror, f),
                        "Can't read 'mirror' surface from io.");
                break;
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_COOK_TORRANCE:
                chk_msg(mli_BoundaryLayer_Surface_Cook_Torrance_from_io(
                                &self->data.cook_torrance, f),
                        "Can't read 'cook-torrance' surface from io.");
                break;
        default:
                chk_bad("surface-type-id is unknown.");
        }
        return 1;
chk_error:
        return 0;
}

int mli_BoundaryLayer_Surface_from_json_string_and_name(
        struct mli_BoundaryLayer_Surface *self,
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
                mli_BoundaryLayer_Surface_type_from_string(&key, &self->type),
                ("Can't map surface 'type':'%s' from json string.", key.array));

        chk_msg(mli_String_copy(&self->name, name), "Can't copy surface name.");

        switch (self->type) {
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_PHONG:
                chk_msg(mli_BoundaryLayer_Surface_Phong_from_json_string(
                                &self->data.phong, spectra_names, json_string),
                        "Can't parse 'phong' surface from json.");
                break;
        case MLI_BOUNDARYLAYER_SURFACE_TYPE_TRANSPARENT:
                chk_msg(mli_BoundaryLayer_Surface_Transparent_from_json_string(
                                &self->data.transparent,
                                spectra_names,
                                json_string),
                        "Can't parse 'transparent' surface from json.");
                break;
        default:
                chk_badf(("surface-type-id '%lu' is unknown.", self->type));
        }

        mli_String_free(&key);
        mli_Json_free(&json);
        return 1;
chk_error:
        return 0;
}
