/* Copyright 2019-2020 Sebastian Achim Mueller                                */

#include "../../mli_testing/src/mli_testing.h"
#include "../src/mliMagicId.h"

CASE("sizeof")
{
        struct mliMagicId magic = mliMagicId_init();
        CHECK(sizeof(magic) == MLI_MAGICID_SIZE);
}

CASE("init")
{
        uint64_t i;
        struct mliMagicId magic = mliMagicId_init();
        CHECK(magic.mayor == MLI_VERSION_MAYOR);
        CHECK(magic.minor == MLI_VERSION_MINOR);
        CHECK(magic.patch == MLI_VERSION_PATCH);
        for (i = 0; i < sizeof(magic.word); i++) {
                CHECK(magic.word[i] == '\0');
        }
}

CASE("set word")
{
        struct mliMagicId magic = mliMagicId_init();
        CHECK(mliMagicId_set(&magic, "something"));
        CHECK(magic.mayor == MLI_VERSION_MAYOR);
        CHECK(magic.minor == MLI_VERSION_MINOR);
        CHECK(magic.patch == MLI_VERSION_PATCH);
        CHECK(strcmp(magic.word, "something") == 0);
}

CASE("set bad word")
{
        struct mliMagicId magic = mliMagicId_init();
        char too_long[] = "way too long can not accept whatever is written "
                          "here is longer than 52chars, bla, bla bla";
        CHECK(strlen(too_long) > MLI_MAGICID_WORD_CAPACITY);
        CHECK(!mliMagicId_set(&magic, too_long));
}

CASE("compare words")
{
        struct mliMagicId magic = mliMagicId_init();
        CHECK(mliMagicId_set(&magic, "alpha"));
        CHECK(mliMagicId_has_word(&magic, "alpha"));
        CHECK(!mliMagicId_has_word(&magic, "beta"));
}
