/* Copyright 2018-2020 Sebastian Achim Mueller */
#ifndef MLIARCHIVE_H_
#define MLIARCHIVE_H_

#include <stdint.h>
#include "../io/io_memory.h"
#include "../array/array.h"
#include "../string/string.h"
#include "../string/string_vector.h"
#include "../map/map.h"

struct mliArchive {
        struct mli_StringVector textfiles;
        struct mli_Map filenames;
};

struct mliArchive mliArchive_init(void);

void mliArchive_free(struct mliArchive *arc);
int mliArchive_malloc(struct mliArchive *arc);
int mliArchive_from_file(struct mliArchive *arc, FILE *f);
int mliArchive_from_path(struct mliArchive *arc, const char *path);
int mliArchive_push_back(
        struct mliArchive *arc,
        const struct mli_String *filename,
        const struct mli_String *payload);
int mliArchive_has(
        const struct mliArchive *arc,
        const struct mli_String *filename);
int mliArchive_get(
        const struct mliArchive *arc,
        const struct mli_String *filename,
        struct mli_String **str);
uint64_t mliArchive_size(const struct mliArchive *arc);
uint64_t mliArchive_num_filename_prefix_sufix(
        const struct mliArchive *arc,
        const char *prefix,
        const char *sufix);

#endif
