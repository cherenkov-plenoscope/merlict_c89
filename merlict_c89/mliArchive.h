/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_ARCHIVE_H_
#define MERLICT_C89_ARCHIVE_H_

#include "mli_debug.h"
#include "mliTar.h"
#include "mli_string_to.h"

MLIDYNARRAY_DEFINITON(mli, String, struct mliString)

struct mliArchive {
        struct mliDynString strings;
        struct mliDynMap filenames;
};

struct mliArchive mliArchive_init(void);

void mliArchive_free(struct mliArchive *arc);
int mliArchive_malloc_from_tar(struct mliArchive *arc, const char *path);

int mliArchive_has(const struct mliArchive *arc, const char *filename);
int mliArchive_get(
        struct mliArchive *arc,
        const char *filename,
        struct mliString **str);
uint64_t mliArchive_num(const struct mliArchive *arc);
void mliArchive_print(struct mliArchive *arc);

#endif
