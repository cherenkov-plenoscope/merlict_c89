/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "boundarylayer_medium.h"

struct mli_BoundaryLayer_Medium mli_BoundaryLayer_Medium_init(void)
{
        struct mli_BoundaryLayer_Medium out;
        out.name = mli_String_init();
        out.type = MLI_BOUNDARYLAYER_MEDIUM_TYPE_NONE;
        return out;
}

int mli_BoundaryLayer_Medium_equal(
        const struct mli_BoundaryLayer_Medium *a,
        const struct mli_BoundaryLayer_Medium *b)
{
        chk_msg(a->type == b->type, "Different types of medium models.");
        chk_msg(mli_String_equal(&a->name, &b->name),
                "Different names of medium models.");

        switch (a->type) {
        case MLI_BOUNDARYLAYER_MEDIUM_TYPE_TRANSPARENT:
                chk_msg(mli_BoundaryLayer_Medium_Transparent_equal(
                                &a->data.transparent, &b->data.transparent),
                        "'transparent' media are not equal.");
                break;
        default:
                chk_bad("medium-type-id is unknown.");
        }

        return 1;
chk_error:
        return 0;
}

int mli_BoundaryLayer_Medium_type_to_string(
        const uint64_t type,
        struct mli_String *s)
{
        switch (type) {
        case MLI_BOUNDARYLAYER_MEDIUM_TYPE_TRANSPARENT:
                chk(mli_String_from_cstr(s, "transparent"));
                break;
        default:
                chk_bad("medium-type-id is unknown.");
        }
        return 1;
chk_error:
        return 0;
}

int mli_BoundaryLayer_Medium_type_from_string(
        const struct mli_String *s,
        uint64_t *id)
{
        if (mli_String_equal_cstr(s, "transparent")) {
                (*id) = MLI_BOUNDARYLAYER_MEDIUM_TYPE_TRANSPARENT;
                return 1;
        } else {
                chk_badf(("medium-type-string '%s' is unknown.", s->array));
        }
        return 1;
chk_error:
        return 0;
}

int mli_BoundaryLayer_Medium_to_io(
        const struct mli_BoundaryLayer_Medium *self,
        struct mli_IO *f)
{
        struct mli_MagicId magic = mli_MagicId_init();
        chk(mli_MagicId_set(&magic, "mli_BoundaryLayer_Medium"));
        chk_IO_write(&magic, sizeof(struct mli_MagicId), 1u, f);

        chk_msg(mli_String_to_io(&self->name, f),
                "Can't write medium.name to io.");
        chk_IO_write(&self->type, sizeof(uint64_t), 1u, f);

        switch (self->type) {
        case MLI_BOUNDARYLAYER_MEDIUM_TYPE_TRANSPARENT:
                chk_msg(mli_BoundaryLayer_Medium_Transparent_to_io(
                                &self->data.transparent, f),
                        "Can't write 'transparent' medium to io.");
                break;
        default:
                chk_bad("medium-type-id is unknown.");
        }
        return 1;
chk_error:
        return 0;
}

int mli_BoundaryLayer_Medium_from_io(
        struct mli_BoundaryLayer_Medium *self,
        struct mli_IO *f)
{
        struct mli_MagicId magic;
        chk_IO_read(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk(mli_MagicId_has_word(&magic, "mli_BoundaryLayer_Medium"));
        mli_MagicId_warn_version(&magic);

        chk_msg(mli_String_from_io(&self->name, f),
                "Can't read medium.name from io.");
        chk_IO_read(&self->type, sizeof(uint64_t), 1u, f);

        switch (self->type) {
        case MLI_BOUNDARYLAYER_MEDIUM_TYPE_TRANSPARENT:
                chk_msg(mli_BoundaryLayer_Medium_Transparent_from_io(
                                &self->data.transparent, f),
                        "Can't read 'transparent' medium from io.");
                break;
        default:
                chk_bad("medium-type-id is unknown.");
        }
        return 1;
chk_error:
        return 0;
}

int mli_BoundaryLayer_Medium_from_json_string_and_name(
        struct mli_BoundaryLayer_Medium *self,
        const struct mli_Map *spectra_names,
        const struct mli_String *json_string,
        const struct mli_String *name)
{
        struct mli_Json json = mli_Json_init();
        struct mli_JsonWalk walk = mli_JsonWalk_init();
        struct mli_String key = mli_String_init();

        chk_msg(mli_Json_from_string(&json, json_string),
                "Can't parse medium from json string.");
        walk = mli_JsonWalk_set(&json);

        chk_msg(mli_JsonWalk_to_key(&walk, "type"),
                "Expected field 'type' in medium json string.");

        chk_msg(mli_JsonWalk_get_string(&walk, &key),
                "Expected field 'type' to hold a string.");

        chk_msg(mli_BoundaryLayer_Medium_type_from_string(&key, &self->type),
                "Can't map medium 'type' from json string.");

        chk_msg(mli_String_copy(&self->name, name), "Can't copy medium name.");

        mli_JsonWalk_to_root(&walk);

        switch (self->type) {
        case MLI_BOUNDARYLAYER_MEDIUM_TYPE_TRANSPARENT:
                chk_msg(mli_BoundaryLayer_Medium_Transparent_from_json_string(
                                &self->data.transparent,
                                spectra_names,
                                json_string),
                        "Can't parse transparent medium from json.");
                break;
        default:
                chk_bad("medium-type-id is unknown.");
        }

        mli_String_free(&key);
        mli_Json_free(&json);
        return 1;
chk_error:
        return 0;
}
