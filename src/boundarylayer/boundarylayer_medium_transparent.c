/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "boundarylayer_medium_transparent.h"
#include "../io/io.h"
#include "../string/string.h"
#include "../map/map.h"
#include "../chk/chk.h"
#include "../magicid/magicid.h"

int mli_BoundaryLayer_Medium_Transparent_equal(
        const struct mli_BoundaryLayerMedium_Transparent *a,
        const struct mli_BoundaryLayerMedium_Transparent *b)
{
        if (a->refraction_spectrum != b->refraction_spectrum) {
                return 0;
        }
        return 1;
}

int mli_BoundaryLayer_Medium_Transparent_to_io(
        const struct mli_BoundaryLayerMedium_Transparent *self,
        struct mli_IO *f)
{
        struct mli_MagicId magic = mli_MagicId_init();
        chk(mli_MagicId_set(&magic, "mli_BoundaryLayerMedium_Transparent"));
        chk_IO_write(&magic, sizeof(struct mli_MagicId), 1u, f);

        chk_IO_write(&self->refraction_spectrum, sizeof(uint64_t), 1u, f);

        return 1;
chk_error:
        return 0;
}

int mli_BoundaryLayer_Medium_Transparent_from_io(
        struct mli_BoundaryLayerMedium_Transparent *self,
        struct mli_IO *f)
{
        struct mli_MagicId magic;
        chk_IO_read(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk(mli_MagicId_has_word(
                &magic, "mli_BoundaryLayerMedium_Transparent"));
        mli_MagicId_warn_version(&magic);

        chk_IO_read(&self->refraction_spectrum, sizeof(uint64_t), 1u, f);

        return 1;
chk_error:
        return 0;
}

int mli_BoundaryLayer_Medium_Transparent_from_json_string(
        struct mli_BoundaryLayerMedium_Transparent *self,
        const struct mli_Map *spectra_names,
        const struct mli_String *json_string)
{
        struct mli_Json json = mli_Json_init();
        struct mli_JsonWalk walk = mli_JsonWalk_init();
        struct mli_String key = mli_String_init();

        chk_msg(mli_Json_from_string(&json, json_string),
                "Can't parse medium from json string.");
        walk = mli_JsonWalk_set(&json);

        chk_msg(mli_JsonWalk_to_key(&walk, "refraction_spectrum"),
                "Expected field 'refraction_spectrum' in transparent "
                "medium json string.");

        chk_msg(mli_JsonWalk_get_string(&walk, &key),
                "Expected 'refraction_spectrum' to hold a string.");

        chk_msg(mli_Map_get(spectra_names, &key, &self->refraction_spectrum),
                "Expected 'refraction_spectrum' to be in spectra_names.");

        mli_String_free(&key);
        mli_Json_free(&json);

        return 1;
chk_error:
        return 0;
}
