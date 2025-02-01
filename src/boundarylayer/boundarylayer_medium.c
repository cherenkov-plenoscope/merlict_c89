/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "boundarylayer_medium.h"

struct mli_BoundaryLayer_Medium mli_BoundaryLayer_Medium_init(void)
{
        struct mli_BoundaryLayer_Medium out;
        out.name = mli_String_init();
        out.refraction_spectrum = 0;
        out.absorbtion_spectrum = 0;
        return out;
}

void mli_BoundaryLayer_Medium_free(struct mli_BoundaryLayer_Medium *self)
{
        mli_String_free(&self->name);
        (*self) = mli_BoundaryLayer_Medium_init();
}

int mli_BoundaryLayer_Medium_valid_wrt_materials(
        const struct mli_BoundaryLayer_Medium *self,
        const struct mli_Materials *materials)
{
        chk_msg(mli_String_valid(&self->name, 1), "name is invalid.");

        chk_msg(self->refraction_spectrum < materials->spectra.size,
                "refraction_spectrum index is not in materials.");
        chk_msg(self->absorbtion_spectrum < materials->spectra.size,
                "absorbtion_spectrum index is not in materials.");

        return 1;
chk_error:
        return 0;
}

int mli_BoundaryLayer_Medium_equal(
        const struct mli_BoundaryLayer_Medium *a,
        const struct mli_BoundaryLayer_Medium *b)
{
        chk_msg(mli_String_equal(&a->name, &b->name),
                "Different names of medium models.");
        chk_msg(a->refraction_spectrum == b->refraction_spectrum,
                "Different refraction_spectrum.");
        chk_msg(a->absorbtion_spectrum == b->absorbtion_spectrum,
                "Different absorbtion_spectrum.");

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
        chk_IO_write(&self->refraction_spectrum, sizeof(int64_t), 1u, f);
        chk_IO_write(&self->absorbtion_spectrum, sizeof(int64_t), 1u, f);

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
        chk_IO_read(&self->refraction_spectrum, sizeof(int64_t), 1u, f);
        chk_IO_read(&self->absorbtion_spectrum, sizeof(int64_t), 1u, f);

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

        mli_BoundaryLayer_Medium_free(self);

        chk_msg(mli_String_copy(&self->name, name), "Can't copy medium name.");

        chk_msg(mli_Json_from_string(&json, json_string),
                "Can't parse medium from json string.");

        walk = mli_JsonWalk_set(&json);
        chk_msg(mli_JsonWalk_to_key(&walk, "refraction_spectrum"),
                "Expected field 'refraction_spectrum' in medium json string.");
        chk_msg(mli_JsonWalk_get_string(&walk, &key),
                "Expected 'refraction_spectrum' to hold a string.");
        chk_msg(mli_Map_get(spectra_names, &key, &self->refraction_spectrum),
                "Expected 'refraction_spectrum' to be in spectra_names.");

        walk = mli_JsonWalk_set(&json);
        chk_msg(mli_JsonWalk_to_key(&walk, "absorbtion_spectrum"),
                "Expected field 'absorbtion_spectrum' in medium json string.");
        chk_msg(mli_JsonWalk_get_string(&walk, &key),
                "Expected 'absorbtion_spectrum' to hold a string.");
        chk_msg(mli_Map_get(spectra_names, &key, &self->absorbtion_spectrum),
                "Expected 'absorbtion_spectrum' to be in spectra_names.");

        mli_String_free(&key);
        mli_Json_free(&json);
        return 1;
chk_error:
        return 0;
}
