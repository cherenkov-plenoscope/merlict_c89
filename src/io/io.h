/* Copyright 2018-2023 Sebastian Achim Mueller */
#ifndef MLI_STREAM_H_
#define MLI_STREAM_H_

#include <stdint.h>
#include "io_file.h"
#include "io_memory.h"
#include "../chk/chk.h"

#define MLI_IO_TYPE_VOID 0
#define MLI_IO_TYPE_FILE 10
#define MLI_IO_TYPE_MEMORY 20

union mli_IoType {
        struct mli_IoFile file;
        struct mli_IoMemory memory;
};

struct mli_IO {
        int type;
        union mli_IoType data;
};

struct mli_IO mli_IO_init(void);
int mli_IO_close(struct mli_IO *self);
int mli_IO_open_memory(struct mli_IO *self);
int mli_IO_open_file(
        struct mli_IO *self,
        const struct mli_String *filename,
        const struct mli_String *mode);
int mli_IO_adopt_file(struct mli_IO *self, FILE *cfile);
int mli_IO__open_file_cstr(
        struct mli_IO *self,
        const char *filename,
        const char *mode);
size_t mli_IO_write(
        const void *ptr,
        const size_t size,
        const size_t count,
        struct mli_IO *self);
size_t mli_IO_read(
        void *ptr,
        const size_t size,
        const size_t count,
        struct mli_IO *self);
void mli_IO_rewind(struct mli_IO *self);
int64_t mli_IO_tell(struct mli_IO *self);
int64_t mli_IO_seek(
        struct mli_IO *self,
        const int64_t offset,
        const int64_t origin);
int mli_IO_eof(const struct mli_IO *self);
int mli_IO_flush(struct mli_IO *self);

#define chk_IO_write(PTR, SIZE_OF_TYPE, NUM, IO)                               \
        {                                                                      \
                const uint64_t num_written =                                   \
                        mli_IO_write(PTR, SIZE_OF_TYPE, NUM, IO);              \
                chk_msg(num_written == NUM, "Can not write to mli_IO.");       \
        }

#define chk_IO_read(PTR, SIZE_OF_TYPE, NUM, IO)                                \
        {                                                                      \
                const uint64_t num_read =                                      \
                        mli_IO_read(PTR, SIZE_OF_TYPE, NUM, IO);               \
                chk_msg(num_read == NUM, "Can not read from mli_IO.");         \
        }

#endif
