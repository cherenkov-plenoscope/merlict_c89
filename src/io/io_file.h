/* Copyright 2018-2023 Sebastian Achim Mueller */
#ifndef MLI_IO_FILE_H_
#define MLI_IO_FILE_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include "../string/string.h"

struct mli_IoFile {
        struct mli_String filename;
        struct mli_String mode;
        FILE *cfile;
};

struct mli_IoFile mli_IoFile_init(void);
int mli_IoFile_close(struct mli_IoFile *self);
int mli_IoFile_open(
        struct mli_IoFile *self,
        const struct mli_String *filename,
        const struct mli_String *mode);
size_t mli_IoFile_write(
        struct mli_IoFile *self,
        const void *ptr,
        const size_t size,
        const size_t count);
size_t mli_IoFile_read(
        struct mli_IoFile *self,
        void *ptr,
        const size_t size,
        const size_t count);
void mli_IoFile_rewind(struct mli_IoFile *self);
int64_t mli_IoFile_tell(struct mli_IoFile *self);
int64_t mli_IoFile_seek(
        struct mli_IoFile *self,
        const int64_t offset,
        const int64_t origin);
int mli_IoFile_eof(const struct mli_IoFile *self);
#endif
