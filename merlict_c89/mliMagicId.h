/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_MLIMAGICID_H_
#define MERLICT_C89_MLIMAGICID_H_

#include <stdint.h>

#define MLI_MAGICID_WORD_CAPACITY 52
#define MLI_MAGICID_SIZE MLI_MAGICID_WORD_CAPACITY + 12

struct mliMagicId {
        char word[MLI_MAGICID_WORD_CAPACITY];
        uint32_t mayor;
        uint32_t minor;
        uint32_t patch;
};

struct mliMagicId mliMagicId_init(void);
int mliMagicId_set(struct mliMagicId *magic, const char *word);
int mliMagicId_has_word(const struct mliMagicId *magic, const char *word);
void mliMagicId_warn_version(const struct mliMagicId *magic);
#endif
