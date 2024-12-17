/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLI_MAGICID_H_
#define MLI_MAGICID_H_

#include <stdint.h>

#define MLI_MAGICID_WORD_CAPACITY 52
#define MLI_MAGICID_SIZE MLI_MAGICID_WORD_CAPACITY + 12

struct mli_MagicId {
        char word[MLI_MAGICID_WORD_CAPACITY];
        uint32_t mayor;
        uint32_t minor;
        uint32_t patch;
};

struct mli_MagicId mli_MagicId_init(void);
int mli_MagicId_set(struct mli_MagicId *magic, const char *word);
int mli_MagicId_has_word(const struct mli_MagicId *magic, const char *word);
void mli_MagicId_warn_version(const struct mli_MagicId *magic);
#endif
