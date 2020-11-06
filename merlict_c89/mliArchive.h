/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_ARCHIVE_H_
#define MERLICT_C89_ARCHIVE_H_

#include "mli_debug.h"
#include "mliTar.h"
#include "mli_string_to.h"

MLIDYNARRAY_DEFINITON(mli, String, struct mliString)

struct mliArc {
        struct mliDynString strings;
        struct mliDynMap filenames;
};

struct mliArc mliArc_init(void);

void mliArc_free(struct mliArc *arc);
int mliArc_malloc_from_tar(struct mliArc *arc, const char *path);

int mliArc_has(const struct mliArc *arc, const char *filename);
int mliArc_get(struct mliArc *arc, const char *filename, struct mliString **str);
uint64_t mliArc_num(const struct mliArc *arc);
void mliArc_print(struct mliArc *arc);

#endif
