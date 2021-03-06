/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MERLICT_C89_ARCHIVE_H_
#define MERLICT_C89_ARCHIVE_H_

#include "mliString.h"
#include "mliJson.h"

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
        const struct mliArchive *arc,
        const char *filename,
        struct mliString **str);
int mliArchive_get_malloc_json(
        const struct mliArchive *arc,
        const char *filename,
        struct mliJson *json);
uint64_t mliArchive_num(const struct mliArchive *arc);
void mliArchive_info_fprint(FILE *f, const struct mliArchive *arc);
void mliArchive_mask_filename_prefix_sufix(
        const struct mliArchive *arc,
        uint64_t *mask,
        const char *prefix,
        const char *suffix);
uint64_t mliArchive_num_filename_prefix_sufix(
        const struct mliArchive *arc,
        const char *prefix,
        const char *sufix);
void _mli_strip_this_dir(char *dst, const char *src);

#endif
