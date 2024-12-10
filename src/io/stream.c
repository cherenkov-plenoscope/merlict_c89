/* Copyright 2018-2023 Sebastian Achim Mueller */
#include "stream.h"
#include <stdio.h>
#include <stdlib.h>
#include "../chk/chk.h"
#include "../math/math.h"

struct mli_Stream mli_Stream_init(void)
{
        struct mli_Stream out;
        out.type = MLI_IO_TYPE_VOID;
        return out;
}

int mli_Stream_open_memory(struct mli_Stream *self)
{
        mli_Stream_close(self);
        self->type = MLI_IO_TYPE_MEMORY;
        self->data.memory = mli_IO_init();
        return mli_IO_open(&self->data.memory);
}

int mli_Stream_open_file(
        struct mli_Stream *self,
        const struct mli_String *filename,
        const struct mli_String *mode)
{
        mli_Stream_close(self);
        self->type = MLI_IO_TYPE_FILE;
        self->data.file = mli_IoFile_init();
        return mli_IoFile_open(&self->data.file, filename, mode);
}

int mli_Stream_close(struct mli_Stream *self)
{
        int rc = 0;
        switch (self->type) {
        case MLI_IO_TYPE_MEMORY:
                rc = mli_IO_close(&self->data.memory);
                break;
        case MLI_IO_TYPE_FILE:
                rc = mli_IoFile_close(&self->data.file);
                break;
        case MLI_IO_TYPE_VOID:
                rc = 1;
                break;
        }
        (*self) = mli_Stream_init();
        return rc;
}

size_t mli_Stream_write(
        struct mli_Stream *self,
        const void *ptr,
        const size_t size,
        const size_t count)
{
        size_t rc = 0;
        switch (self->type) {
        case MLI_IO_TYPE_MEMORY:
                rc = mli_IO_write(&self->data.memory, ptr, size, count);
                break;
        case MLI_IO_TYPE_FILE:
                rc = mli_IoFile_write(&self->data.file, ptr, size, count);
                break;
        default:
                rc = 0;
        }
        return rc;
}

size_t mli_Stream_read(
        struct mli_Stream *self,
        void *ptr,
        const size_t size,
        const size_t count)
{
        size_t rc;
        switch (self->type) {
        case MLI_IO_TYPE_MEMORY:
                rc = mli_IO_read(&self->data.memory, ptr, size, count);
                break;
        case MLI_IO_TYPE_FILE:
                rc = mli_IoFile_read(&self->data.file, ptr, size, count);
                break;
        default:
                rc = 0;
        }
        return rc;
}

void mli_Stream_rewind(struct mli_Stream *self)
{
        switch (self->type) {
        case MLI_IO_TYPE_MEMORY:
                mli_IO_rewind(&self->data.memory);
                break;
        case MLI_IO_TYPE_FILE:
                rewind(self->data.file.cfile);
                break;
        }
}

int64_t mli_Stream_tell(struct mli_Stream *self)
{
        int64_t rc = -1;
        switch (self->type) {
        case MLI_IO_TYPE_MEMORY:
                rc = mli_IO_tell(&self->data.memory);
                break;
        case MLI_IO_TYPE_FILE:
                rc = mli_IoFile_tell(&self->data.file);
                break;
        }
        return rc;
}

int64_t mli_Stream_seek(
        struct mli_Stream *self,
        const int64_t offset,
        const int64_t origin)
{
        int64_t rc = -1;
        switch (self->type) {
        case MLI_IO_TYPE_MEMORY:
                rc = mli_IO_seek(&self->data.memory, offset, origin);
                break;
        case MLI_IO_TYPE_FILE:
                rc = mli_IoFile_seek(&self->data.file, offset, origin);
                break;
        }
        return rc;
}
