/* Copyright 2018-2023 Sebastian Achim Mueller */
#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include "../chk/chk.h"
#include "../math/math.h"

struct mli_IO mli_IO_init(void)
{
        struct mli_IO out;
        out.type = MLI_IO_TYPE_VOID;
        return out;
}

chk_rc mli_IO_open_memory(struct mli_IO *self)
{
        mli_IO_close(self);
        self->type = MLI_IO_TYPE_MEMORY;
        self->data.memory = mli_IoMemory_init();
        return mli_IoMemory_open(&self->data.memory);
}

chk_rc mli_IO_open_file(
        struct mli_IO *self,
        const struct mli_String *filename,
        const struct mli_String *mode)
{
        mli_IO_close(self);
        self->type = MLI_IO_TYPE_FILE;
        self->data.file = mli_IoFile_init();
        return mli_IoFile_open(&self->data.file, filename, mode);
}

chk_rc mli_IO_adopt_file(struct mli_IO *self, FILE *cfile)
{
        mli_IO_close(self);
        self->type = MLI_IO_TYPE_FILE;
        self->data.file = mli_IoFile_init();
        return mli_IoFile_adopt_cfile(&self->data.file, cfile);
}

chk_rc mli_IO_open_file_cstr(
        struct mli_IO *self,
        const char *filename,
        const char *mode)
{
        struct mli_String _filename = mli_String_init();
        struct mli_String _mode = mli_String_init();

        chk(mli_String_from_cstr(&_filename, filename));
        chk(mli_String_from_cstr(&_mode, mode));
        chk(mli_IO_open_file(self, &_filename, &_mode));

        mli_String_free(&_filename);
        mli_String_free(&_mode);
        return CHK_SUCCESS;
chk_error:
        mli_String_free(&_filename);
        mli_String_free(&_mode);
        mli_IO_close(self);
        return CHK_FAIL;
}

chk_rc mli_IO_close(struct mli_IO *self)
{
        chk_rc rc = CHK_FAIL;
        switch (self->type) {
        case MLI_IO_TYPE_MEMORY:
                rc = mli_IoMemory_close(&self->data.memory);
                break;
        case MLI_IO_TYPE_FILE:
                rc = mli_IoFile_close(&self->data.file);
                break;
        case MLI_IO_TYPE_VOID:
                rc = CHK_FAIL;
                break;
        }
        (*self) = mli_IO_init();
        return rc;
}

size_t mli_IO_write(
        const void *ptr,
        const size_t size,
        const size_t count,
        struct mli_IO *self)
{
        size_t rc = 0;
        switch (self->type) {
        case MLI_IO_TYPE_MEMORY:
                rc = mli_IoMemory_write(ptr, size, count, &self->data.memory);
                break;
        case MLI_IO_TYPE_FILE:
                rc = mli_IoFile_write(ptr, size, count, &self->data.file);
                break;
        case MLI_IO_TYPE_VOID:
                chk_warning("Expected io->type != VOID in order to write.");
                rc = 0;
        }
        return rc;
}

size_t mli_IO_read(
        void *ptr,
        const size_t size,
        const size_t count,
        struct mli_IO *self)
{
        size_t rc;
        switch (self->type) {
        case MLI_IO_TYPE_MEMORY:
                rc = mli_IoMemory_read(ptr, size, count, &self->data.memory);
                break;
        case MLI_IO_TYPE_FILE:
                rc = mli_IoFile_read(ptr, size, count, &self->data.file);
                break;
        case MLI_IO_TYPE_VOID:
                chk_warning("Expected io->type != VOID in order to read.");
                rc = 0;
        }
        return rc;
}

void mli_IO_rewind(struct mli_IO *self)
{
        switch (self->type) {
        case MLI_IO_TYPE_MEMORY:
                mli_IoMemory_rewind(&self->data.memory);
                break;
        case MLI_IO_TYPE_FILE:
                rewind(self->data.file.cfile);
                break;
        case MLI_IO_TYPE_VOID:
                chk_warning("Expected io->type != VOID in order to rewind.");
        }
}

int64_t mli_IO_tell(struct mli_IO *self)
{
        int64_t rc = EOF;
        switch (self->type) {
        case MLI_IO_TYPE_MEMORY:
                rc = mli_IoMemory_tell(&self->data.memory);
                break;
        case MLI_IO_TYPE_FILE:
                rc = mli_IoFile_tell(&self->data.file);
                break;
        case MLI_IO_TYPE_VOID:
                rc = EOF;
                chk_warning("Expected io->type != VOID in order to tell.");
        }
        return rc;
}

int64_t mli_IO_seek(
        struct mli_IO *self,
        const int64_t offset,
        const int64_t origin)
{
        int64_t rc = EOF;
        switch (self->type) {
        case MLI_IO_TYPE_MEMORY:
                rc = mli_IoMemory_seek(&self->data.memory, offset, origin);
                break;
        case MLI_IO_TYPE_FILE:
                rc = mli_IoFile_seek(&self->data.file, offset, origin);
                break;
        case MLI_IO_TYPE_VOID:
                rc = EOF;
                chk_warning("Expected io->type != VOID in order to seek.");
        }
        return rc;
}

int64_t mli_IO_eof(const struct mli_IO *self)
{
        int64_t rc = EOF;
        switch (self->type) {
        case MLI_IO_TYPE_MEMORY:
                rc = mli_IoMemory_eof(&self->data.memory);
                break;
        case MLI_IO_TYPE_FILE:
                rc = mli_IoFile_eof(&self->data.file);
                break;
        case MLI_IO_TYPE_VOID:
                rc = EOF;
                chk_warning("Expected io->type != VOID in order to eof.");
        }
        return rc;
}

int64_t mli_IO_flush(struct mli_IO *self)
{
        int64_t rc = EOF;
        switch (self->type) {
        case MLI_IO_TYPE_MEMORY:
                rc = 0;
                break;
        case MLI_IO_TYPE_FILE:
                rc = mli_IoFile_flush(&self->data.file);
                break;
        case MLI_IO_TYPE_VOID:
                rc = EOF;
                chk_warning("Expected io->type != VOID in order to flush.");
        }
        return rc;
}
