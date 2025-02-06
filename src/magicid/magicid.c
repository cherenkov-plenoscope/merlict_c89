/* Copyright 2018-2020 Sebastian Achim Mueller */
#include "magicid.h"
#include <assert.h>
#include "../version/version.h"
#include "../math/math.h"

struct mli_MagicId mli_MagicId_init(void)
{
        struct mli_MagicId magic;
        memset(magic.word, '\0', sizeof(magic.word));
        magic.mayor = MLI_VERSION_MAYOR;
        magic.minor = MLI_VERSION_MINOR;
        magic.patch = MLI_VERSION_PATCH;
        return magic;
}

chk_rc mli_MagicId_set(struct mli_MagicId *magic, const char *word)
{
        uint64_t i, len;
        (*magic) = mli_MagicId_init();
        chk_msg(strlen(word) < sizeof(magic->word),
                "Expected magic word to be shorter.");

        len = MLI_MATH_MIN2(strlen(word), sizeof(magic->word));

        for (i = 0; i < len; i++) {
                magic->word[i] = word[i];
        }
        while (i < sizeof(magic->word)) {
                magic->word[i] = '\0';
                i += 1;
        }
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_MagicId_has_word(const struct mli_MagicId *magic, const char *word)
{
        uint64_t i, len;
        chk_msg(strlen(word) < sizeof(magic->word),
                "Expected magic word to be shorter.");

        len = MLI_MATH_MIN2(strlen(word), sizeof(magic->word));

        for (i = 0; i < len; i++) {
                if (magic->word[i] != word[i]) {
                        return CHK_FAIL;
                }
        }
        while (i < sizeof(magic->word)) {
                if (magic->word[i] != '\0') {
                        return CHK_FAIL;
                }
                i += 1;
        }
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

void mli_MagicId_warn_version(const struct mli_MagicId *magic)
{
        if ((magic->mayor != MLI_VERSION_MAYOR) ||
            (magic->minor != MLI_VERSION_MINOR) ||
            (magic->patch != MLI_VERSION_PATCH)) {
                fprintf(stderr,
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
