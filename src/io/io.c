/* Copyright 2018-2023 Sebastian Achim Mueller */
#include "io.h"
#include <stdio.h>
#include <stdlib.h>
#include "../chk/chk.h"
#include "../math/math.h"

struct mli_IO mli_IO_init(void)
{
        struct mli_IO byt;
        byt.cstr = NULL;
        byt.capacity = 0u;
        byt.size = 0u;
        byt.pos = 0u;
        return byt;
}

int mli_IO_close(struct mli_IO *self)
{
        const int RC_IS_ALWAYS_1 = 1;
        free(self->cstr);
        (*self) = mli_IO_init();
        return RC_IS_ALWAYS_1;
}

int mli_IO__malloc_capacity(struct mli_IO *self, const uint64_t capacity)
{
        mli_IO_close(self);
        self->capacity = MLI_MATH_MAX2(2u, capacity);
        self->size = 0u;
        chk_malloc(self->cstr, unsigned char, self->capacity + 1u);
        memset(self->cstr, '\0', self->capacity + 1u);
        return 1;
chk_error:
        return 0;
}

int mli_IO__malloc(struct mli_IO *self)
{
        chk(mli_IO__malloc_capacity(self, 0u));
        return 1;
chk_error:
        return 0;
}

int mli_IO__realloc_capacity(struct mli_IO *self, const uint64_t new_capacity)
{
        uint64_t numcpy;
        struct mli_IO tmp = mli_IO_init();

        chk_msg(mli_IO__malloc_capacity(&tmp, self->capacity),
                "Failed to allocate temporary swap.");
        memcpy(tmp.cstr, self->cstr, self->capacity);
        tmp.pos = self->pos;
        tmp.size = self->size;

        chk_msg(mli_IO__malloc_capacity(self, new_capacity),
                "Faild to allocate new capacity.");

        if (new_capacity >= tmp.capacity) {
                /* growing or same */
                numcpy = tmp.capacity;
        } else {
                /* shrinking */
                numcpy = new_capacity;
                chk_msg(tmp.pos <= new_capacity,
                        "Expected cursor 'pos' to be within new capacity.");
                chk_msg(tmp.size <= new_capacity,
                        "Expected 'size' to be within new capacity.");
        }

        memcpy(self->cstr, tmp.cstr, numcpy);
        self->pos = tmp.pos;
        self->size = tmp.size;

        mli_IO_close(&tmp);
        return 1;
chk_error:
        return 0;
}

int mli_IO_open(struct mli_IO *self)
{
        if (self->cstr == NULL) {
                chk(mli_IO__malloc(self));
        } else {
                chk_msg(self->capacity >= 2u,
                        "Expected minimal capacity of 2.");
                self->pos = 0u;
                self->size = 0u;
                memset(self->cstr, '\0', self->capacity + 1u);
        }
        return 1;
chk_error:
        return 0;
}

int mli_IO__shrink_to_fit(struct mli_IO *self)
{
        chk_msg(mli_IO__realloc_capacity(self, self->size),
                "Failed to reallocate to size.");
        return 1;
chk_error:
        return 0;
}

int mli_IO__write_unsigned_char(struct mli_IO *self, const unsigned char *c)
{
        /* 'puts' a single selfe (char) to the selfesIo-buffer */
        const uint64_t new_size = self->size + 1u;

        if (self->cstr == NULL) {
                chk(mli_IO__malloc(self));
        }

        if (new_size >= self->capacity) {
                const uint64_t min_new_capacity =
                        MLI_MATH_MAX2(new_size, 2 * self->capacity);
                chk_msg(mli_IO__realloc_capacity(self, min_new_capacity),
                        "Failed to reallocate.");
        }
        self->cstr[self->size] = (*c);
        self->size = new_size;
        self->pos += 1;

        return 1;
chk_error:
        return 0;
}

int mli_IO__read_unsigned_char(struct mli_IO *self, unsigned char *c)
{
        if (self->pos >= self->size) {
                return EOF;
        }
        (*c) = self->cstr[self->pos];
        self->pos += 1;
        return 1;
}

size_t mli_IO_write(
        struct mli_IO *self,
        const void *ptr,
        const size_t size,
        const size_t count)
{
        const size_t block_size = size * count;
        unsigned char *block = (unsigned char *)ptr;

        size_t i;
        for (i = 0u; i < block_size; i++) {
                chk_msg(mli_IO__write_unsigned_char(self, &block[i]), "");
        }
chk_error:
        return (i + 1u) / size;
}

size_t mli_IO_read(
        struct mli_IO *self,
        void *ptr,
        const size_t size,
        const size_t count)
{
        const size_t block_size = size * count;
        unsigned char *block = (unsigned char *)ptr;

        size_t i;
        for (i = 0u; i < block_size; i++) {
                unsigned char c;
                const int rc = mli_IO__read_unsigned_char(self, &c);
                if (rc == EOF) {
                        return i / size;
                        ;
                } else {
                        block[i] = c;
                }
        }
        return (i + 1u) / size;
}

void mli_IO_rewind(struct mli_IO *self) { self->pos = 0u; }

int64_t mli_IO_tell(struct mli_IO *self) { return (int64_t)self->pos; }

int64_t mli_IO_seek(
        struct mli_IO *self,
        const int64_t offset,
        const int64_t origin)
{
        const int64_t SUCCESS = 0;
        const int64_t FAIL = -1;
        /* If successful, the function returns zero.
         * Otherwise, it returns non-zero value.
         */
        if (origin == SEEK_CUR) {
                const int64_t new_pos = self->pos + offset;
                return mli_IO_seek(self, new_pos, SEEK_SET);
        } else if (origin == SEEK_SET) {
                if (offset < 0 || offset >= (int64_t)self->size) {
                        return FAIL;
                } else {
                        self->pos = offset;
                        return SUCCESS;
                }
        } else {
                return FAIL;
        }
}

int mli_IO_write_from_path(struct mli_IO *self, const char *path)
{
        int rc;
        unsigned char c;
        FILE *f = fopen(path, "rt");
        chk_msg(f, "Failed to open file.");
        chk_msg(mli_IO__malloc(self), "Can not malloc string.");
        while (1) {
                rc = fread((void *)(&c), sizeof(unsigned char), 1, f);
                if (rc == 0) {
                        break;
                }
                chk_msg(mli_IO__write_unsigned_char(self, &c),
                        "Failed to write char to IO.");
        }
        fclose(f);
        return 1;
chk_error:
        if (f != NULL)
                fclose(f);
        mli_IO_close(self);
        return 0;
}

int mli_IO_read_to_path(struct mli_IO *self, const char *path)
{
        FILE *f = fopen(path, "w");
        chk_msg(f != NULL, "Failed to open path.");
        while (1) {
                unsigned char c;
                int rc = mli_IO__read_unsigned_char(self, &c);
                if (rc == EOF) {
                        break;
                }
                chk_fwrite(&c, sizeof(unsigned char), 1, f);
        }
        fclose(f);
        return 1;
chk_error:
        fclose(f);
        return 0;
}
