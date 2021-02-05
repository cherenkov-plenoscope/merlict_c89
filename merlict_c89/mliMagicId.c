/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "mliMagicId.h"
#include <assert.h>
#include "mli_debug.h"

struct mliMagicId mliMagicId_init(void) {
        struct mliMagicId magic;
        memset(magic.word, '\0', sizeof(magic.word));
        magic.mayor = MLI_VERSION_MAYOR;
        magic.minor = MLI_VERSION_MINOR;
        magic.patch = MLI_VERSION_PATCH;
        return magic;
}

int mliMagicId_set(struct mliMagicId *magic, const char *word)
{
        uint64_t i, len;
        (*magic) = mliMagicId_init();
        mli_check(
                strlen(word) < sizeof(magic->word),
                "Expected magic word to be shorter than 20 chars.");

        len = MLI_MIN2(strlen(word), sizeof(magic->word));

        for (i = 0; i < len; i++) {
                magic->word[i] = word[i];
        }
        while (i < sizeof(magic->word)) {
                magic->word[i] = '\0';
                i += 1;
        }
        return 1;
error:
        return 0;
}

int mliMagicId_has_word(const struct mliMagicId *magic, const char *word)
{
        uint64_t i, len;
        mli_check(
                strlen(word) < sizeof(magic->word),
                "Expected magic word to be shorter than 20 chars.");

        len = MLI_MIN2(strlen(word), sizeof(magic->word));

        for (i = 0; i < len; i++) {
                if (magic->word[i] != word[i]) {
                        return 0;
                }
        }
        while (i < sizeof(magic->word)) {
                if (magic->word[i] != '\0') {
                        return 0;
                }
                i += 1;
        }
        return 1;
error:
        return 0;
}

void mliMagicId_warn_version(const struct mliMagicId *magic)
{
        if (
                (magic->mayor != MLI_VERSION_MAYOR)
                ||
                (magic->minor != MLI_VERSION_MINOR)
                ||
                (magic->patch != MLI_VERSION_PATCH)
        ) {
                fprintf(
                        stderr,
                        "[WARNING] Expected magic version to be %d.%d.%d, "
                        "but it is %d.%d.%d.\n",
                        MLI_VERSION_MAYOR,
                        MLI_VERSION_MINOR,
                        MLI_VERSION_PATCH,
                        magic->mayor,
                        magic->minor,
                        magic->patch);
        }
}
