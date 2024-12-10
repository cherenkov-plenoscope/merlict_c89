/* Copyright 2018-2023 Sebastian Achim Mueller */
#ifndef MLI_STREAM_H_
#define MLI_STREAM_H_

#include <stdint.h>
#include "io_file.h"
#include "io.h"

#define MLI_IO_TYPE_VOID 0
#define MLI_IO_TYPE_FILE 10
#define MLI_IO_TYPE_MEMORY 20

union mli_IoType {
        struct mli_IoFile file;
        struct mli_IO memory;
};

struct mli_Stream {
        int type;
        union mli_IoType data;
};

struct mli_Stream mli_Stream_init(void);
int mli_Stream_close(struct mli_Stream *self);
int mli_Stream_open_memory(struct mli_Stream *self);
int mli_Stream_open_file(
        struct mli_Stream *self,
        const struct mli_String *filename,
        const struct mli_String *mode);
size_t mli_Stream_write(
        struct mli_Stream *self,
        const void *ptr,
        const size_t size,
        const size_t count);
size_t mli_Stream_read(
        struct mli_Stream *self,
        void *ptr,
        const size_t size,
        const size_t count);
void mli_Stream_rewind(struct mli_Stream *self);

#endif
