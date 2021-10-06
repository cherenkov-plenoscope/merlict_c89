/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliMedium.h"

struct mliMedium mliMedium_init(void)
{
        struct mliMedium medium;
        medium.refraction = mliFunc_init();
        medium.absorbtion = mliFunc_init();
        return medium;
}

void mliMedium_free(struct mliMedium *medium)
{
        mliFunc_free(&medium->refraction);
        mliFunc_free(&medium->absorbtion);
        (*medium) = mliMedium_init();
}

int mliMedium_equal(const struct mliMedium *a, const struct mliMedium *b)
{
        if (!mliFunc_equal(a->refraction, b->refraction))
                return 0;
        if (!mliFunc_equal(a->absorbtion, b->absorbtion))
                return 0;
        return 1;
}

int mliMedium_fwrite(const struct mliMedium *med, FILE *f)
{
        struct mliMagicId magic = mliMagicId_init();
        /* magic identifier */
        chk(mliMagicId_set(&magic, "mliMedium"));
        chk_fwrite(&magic, sizeof(struct mliMagicId), 1u, f);

        chk(mliFunc_fwrite(&med->refraction, f));
        chk(mliFunc_fwrite(&med->absorbtion, f));

        return 1;
error:
        return 0;
}

int mliMedium_malloc_fread(struct mliMedium *med, FILE *f)
{
        struct mliMagicId magic;
        /* magic identifier */
        chk_fread(&magic, sizeof(struct mliMagicId), 1u, f);
        chk(mliMagicId_has_word(&magic, "mliMedium"));
        mliMagicId_warn_version(&magic);

        chk(mliFunc_malloc_fread(&med->refraction, f));
        chk(mliFunc_malloc_fread(&med->absorbtion, f));

        return 1;
error:
        return 0;
}

int mliMedium_malloc_from_json_str(struct mliMedium *med, const char *json_str)
{
        struct mliJson json = mliJson_init();
        chk_msg(mliJson_malloc_from_string(&json, json_str),
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
        chk_msg(mliJson_find_key(json, token, "refraction", &refraction_token),
                "Expected medium to have key 'refraction', but it does not.");
        chk_msg(mliFunc_malloc_from_json_token(
                        &med->refraction, json, refraction_token + 1),
                "Failed to read medium's refraction from json.");
        chk_msg(mliJson_find_key(json, token, "absorbtion", &absorbtion_token),
                "Expected medium to have key 'absorbtion', but it does not.");
        chk_msg(mliFunc_malloc_from_json_token(
                        &med->absorbtion, json, absorbtion_token + 1),
                "Failed to read medium's absorbtion from json.");
        return 1;
error:
        return 0;
}
