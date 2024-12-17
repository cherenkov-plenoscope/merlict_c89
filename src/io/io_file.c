/* Copyright 2018-2023 Sebastian Achim Mueller */
#include "io_file.h"
#include "../chk/chk.h"

struct mli_IoFile mli_IoFile_init(void)
{
        struct mli_IoFile out;
        out.filename = mli_String_init();
        out.mode = mli_String_init();
        out.cfile = NULL;
        return out;
}

int mli_IoFile_close(struct mli_IoFile *self)
{
        int rc = 1;
        mli_String_free(&self->filename);
        mli_String_free(&self->mode);
        if (self->cfile != NULL) {
                int fclose_rc = fclose(self->cfile);
                if (fclose_rc == EOF) {
                        rc = EOF;
                }
        }

        (*self) = mli_IoFile_init();
        return rc;
}

int mli_IoFile_open(
        struct mli_IoFile *self,
        const struct mli_String *filename,
        const struct mli_String *mode)
{
        mli_IoFile_close(self);
        chk_msg(mli_String_copy(&self->filename, filename),
                "Can not copy filename.");
        chk_msg(mli_String_copy(&self->mode, mode), "Can not copy mode.");
        self->cfile = fopen(self->filename.array, self->mode.array);
        chk_msg(self->cfile != NULL, "Failed to open file.");
        return 1;
chk_error:
        mli_IoFile_close(self);
        return 0;
}

size_t mli_IoFile_write(
        struct mli_IoFile *self,
        const void *ptr,
        const size_t size,
        const size_t count)
{
        const size_t actual_count = fwrite(ptr, size, count, self->cfile);
        chk_msg(actual_count == count, "Can not write to file.");

chk_error:
        return actual_count;
}

size_t mli_IoFile_read(
        struct mli_IoFile *self,
        void *ptr,
        const size_t size,
        const size_t count)
{
        const size_t actual_count = fread(ptr, size, count, self->cfile);
        chk_msg(actual_count == count, "Can not read from file.");

chk_error:
        return actual_count;
}

void mli_IoFile_rewind(struct mli_IoFile *self) { rewind(self->cfile); }

int64_t mli_IoFile_tell(struct mli_IoFile *self) { return ftell(self->cfile); }

int64_t mli_IoFile_seek(
        struct mli_IoFile *self,
        const int64_t offset,
        const int64_t origin)
{
        return fseek(self->cfile, offset, origin);
}

int mli_IoFile_eof(const struct mli_IoFile *self) { return feof(self->cfile); }
