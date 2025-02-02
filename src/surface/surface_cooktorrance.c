/* Copyright 2018-2024 Sebastian Achim Mueller */
#include "surface_cooktorrance.h"
#include "../chk/chk.h"
#include "../io/io.h"
#include "../magicid/magicid.h"
#include "../json/json.h"
#include "../json/json_walk.h"
#include "../map/map.h"
#include "../materials/materials.h"

int mli_Surface_CookTorrance_equal(
        const struct mli_Surface_CookTorrance *a,
        const struct mli_Surface_CookTorrance *b)
{
        if (a->reflection_spectrum != b->reflection_spectrum) {
                return 0;
        }
        if (a->diffuse_weight != b->diffuse_weight) {
                return 0;
        }
        if (a->specular_weight != b->specular_weight) {
                return 0;
        }
        if (a->roughness != b->roughness) {
                return 0;
        }
        return 1;
}

int mli_Surface_CookTorrance_to_io(
        const struct mli_Surface_CookTorrance *self,
        struct mli_IO *f)
{
        struct mli_MagicId magic = mli_MagicId_init();
        chk(mli_MagicId_set(&magic, "mli_Surface_CookTorrance"));
        chk_IO_write(&magic, sizeof(struct mli_MagicId), 1u, f);

        chk_IO_write(&self->reflection_spectrum, sizeof(uint64_t), 1u, f);
        chk_IO_write(&self->diffuse_weight, sizeof(double), 1u, f);
        chk_IO_write(&self->specular_weight, sizeof(double), 1u, f);
        chk_IO_write(&self->roughness, sizeof(double), 1u, f);

        return 1;
chk_error:
        return 0;
}

int mli_Surface_CookTorrance_from_io(
        struct mli_Surface_CookTorrance *self,
        struct mli_IO *f)
{
        struct mli_MagicId magic;
        chk_IO_read(&magic, sizeof(struct mli_MagicId), 1u, f);
        chk(mli_MagicId_has_word(&magic, "mli_Surface_CookTorrance"));
        mli_MagicId_warn_version(&magic);

        chk_IO_read(&self->reflection_spectrum, sizeof(uint64_t), 1u, f);
        chk_IO_read(&self->diffuse_weight, sizeof(double), 1u, f);
        chk_IO_read(&self->specular_weight, sizeof(double), 1u, f);
        chk_IO_read(&self->roughness, sizeof(double), 1u, f);

        return 1;
chk_error:
        return 0;
}

int mli_Surface_CookTorrance_from_json_string(
        struct mli_Surface_CookTorrance *self,
        const struct mli_Map *spectra_names,
        const struct mli_String *json_string)
{
        struct mli_Json json = mli_Json_init();
        struct mli_JsonWalk walk = mli_JsonWalk_init();
        struct mli_String key = mli_String_init();

        chk_msg(mli_Json_from_string(&json, json_string),
                "Can't parse cook-torrance surface from json string.");
        walk = mli_JsonWalk_set(&json);

        chk_msg(mli_JsonWalk_to_key(&walk, "reflection_spectrum"),
                "Expected field 'reflection_spectrum' in cook-torrance "
                "surface json string.");
        chk_msg(mli_JsonWalk_get_string(&walk, &key),
                "Expected 'reflection_spectrum' to hold a string.");
        chk_msg(mli_Map_get(spectra_names, &key, &self->reflection_spectrum),
                "Expected 'reflection_spectrum' to be in spectra_names.");

        mli_JsonWalk_to_root(&walk);
        chk_msg(mli_JsonWalk_to_key(&walk, "diffuse_weight"),
                "Expected field 'diffuse_weight' in cook-torrance.");
        chk_msg(mli_JsonWalk_get_double(&walk, &self->diffuse_weight),
                "Expected 'diffuse_weight' to hold a double.");

        mli_JsonWalk_to_root(&walk);
        chk_msg(mli_JsonWalk_to_key(&walk, "specular_weight"),
                "Expected field 'specular_weight' in cook-torrance.");
        chk_msg(mli_JsonWalk_get_double(&walk, &self->specular_weight),
                "Expected 'specular_weight' to hold a double.");

        mli_JsonWalk_to_root(&walk);
        chk_msg(mli_JsonWalk_to_key(&walk, "roughness"),
                "Expected field 'roughness' in cook-torrance.");
        chk_msg(mli_JsonWalk_get_double(&walk, &self->roughness),
                "Expected 'roughness' to hold a double.");

        mli_String_free(&key);
        mli_Json_free(&json);

        return 1;
chk_error:
        return 0;
}

int mli_Surface_CookTorrance_valid_wrt_materials(
        const struct mli_Surface_CookTorrance *self,
        const struct mli_Materials *materials)
{
        chk_msg(self->reflection_spectrum < materials->spectra.size,
                "reflection_spectrum index is not in materials.");

        chk_msg(self->diffuse_weight >= 0.0, "Expected diffuse_weight >= 0.0");
        chk_msg(self->diffuse_weight <= 1.0, "Expected diffuse_weight <= 1.0");

        chk_msg(self->specular_weight >= 0.0,
                "Expected specular_weight >= 0.0");
        chk_msg(self->specular_weight <= 1.0,
                "Expected specular_weight <= 1.0");

        chk_msg(self->specular_weight + self->diffuse_weight <= 1.0,
                "Expected (specular_weight + diffuse_weight) <= 1.0");

        chk_msg(self->roughness >= 0.0, "Expected roughness >= 0.0");
        chk_msg(self->roughness <= 1.0, "Expected roughness <= 1.0");

        return 1;
chk_error:
        return 0;
}
