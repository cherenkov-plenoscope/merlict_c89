/* Copyright 2018-2023 Sebastian Achim Mueller */
#include "io_memory.h"
#include <stdlib.h>
#include "../chk/chk.h"
#include "../math/math.h"

struct mli_IoMemory mli_IoMemory_init(void)
{
        struct mli_IoMemory byt;
        byt.cstr = NULL;
        byt.capacity = 0u;
        byt.size = 0u;
        byt.pos = 0u;
        return byt;
}

chk_rc mli_IoMemory_close(struct mli_IoMemory *self)
{
        free(self->cstr);
        (*self) = mli_IoMemory_init();
        return CHK_SUCCESS;
}

chk_rc mli_IoMemory__malloc_capacity(
        struct mli_IoMemory *self,
        const uint64_t capacity)
{
        mli_IoMemory_close(self);
        self->capacity = MLI_MATH_MAX2(2u, capacity);
        self->size = 0u;
        chk_malloc(self->cstr, unsigned char, self->capacity + 1u);
        memset(self->cstr, '\0', self->capacity + 1u);
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_IoMemory__malloc(struct mli_IoMemory *self)
{
        chk(mli_IoMemory__malloc_capacity(self, 0u));
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_IoMemory__realloc_capacity(
        struct mli_IoMemory *self,
        const uint64_t new_capacity)
{
        uint64_t numcpy;
        struct mli_IoMemory tmp = mli_IoMemory_init();

        chk_msg(mli_IoMemory__malloc_capacity(&tmp, self->capacity),
                "Failed to allocate temporary swap.");
        memcpy(tmp.cstr, self->cstr, self->capacity);
        tmp.pos = self->pos;
        tmp.size = self->size;

        chk_msg(mli_IoMemory__malloc_capacity(self, new_capacity),
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

        mli_IoMemory_close(&tmp);
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_IoMemory_open(struct mli_IoMemory *self)
{
        if (self->cstr == NULL) {
                chk(mli_IoMemory__malloc(self));
        } else {
                chk_msg(self->capacity >= 2u,
                        "Expected minimal capacity of 2.");
                self->pos = 0u;
                self->size = 0u;
                memset(self->cstr, '\0', self->capacity + 1u);
        }
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_IoMemory__shrink_to_fit(struct mli_IoMemory *self)
{
        chk_msg(mli_IoMemory__realloc_capacity(self, self->size),
                "Failed to reallocate to size.");
        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_IoMemory__write_unsigned_char(
        struct mli_IoMemory *self,
        const unsigned char *c)
{
        /* 'puts' a single selfe (char) to the selfesIo-buffer */
        uint64_t new_size;

        if (self->pos == self->size) {
                new_size = self->size + 1u;
        } else {
                new_size = self->size;
        }

        if (self->cstr == NULL) {
                chk_msg(mli_IoMemory__malloc(self), "Failed to malloc.");
        }

        if (new_size >= self->capacity) {
                const uint64_t min_new_capacity =
                        MLI_MATH_MAX2(new_size, 2 * self->capacity);
                chk_msg(mli_IoMemory__realloc_capacity(self, min_new_capacity),
                        "Failed to reallocate.");
        }
        self->cstr[self->pos] = (*c);
        self->size = new_size;
        self->pos += 1;

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}

chk_rc mli_IoMemory__read_unsigned_char(
        struct mli_IoMemory *self,
        unsigned char *c)
{
        if (self->pos >= self->size) {
                return EOF;
        }
        (*c) = self->cstr[self->pos];
        self->pos += 1;
        return 1;
}

size_t mli_IoMemory_write(
        const void *ptr,
        const size_t size,
        const size_t count,
        struct mli_IoMemory *self)
{
        const size_t block_size = size * count;
        unsigned char *block = (unsigned char *)ptr;

        size_t i;
        size_t num_written = 0;
        for (i = 0u; i < block_size; i++) {
                chk_msg(mli_IoMemory__write_unsigned_char(self, &block[i]),
                        "Failed to write char block to mli_IoMemory");
        }
        num_written = i / size;
chk_error:
        return num_written;
}

size_t mli_IoMemory_read(
        void *ptr,
        const size_t size,
        const size_t count,
        struct mli_IoMemory *self)
{
        const size_t block_size = size * count;
        unsigned char *block = (unsigned char *)ptr;

        size_t i;
        for (i = 0u; i < block_size; i++) {
                unsigned char c;
                const int rc = mli_IoMemory__read_unsigned_char(self, &c);
                if (rc == EOF) {
                        return i / size;
                        ;
                } else {
                        block[i] = c;
                }
        }
        return i / size;
}

void mli_IoMemory_rewind(struct mli_IoMemory *self) { self->pos = 0u; }

int64_t mli_IoMemory_tell(struct mli_IoMemory *self)
{
        return (int64_t)self->pos;
}

int64_t mli_IoMemory_seek(
        struct mli_IoMemory *self,
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
                return mli_IoMemory_seek(self, new_pos, SEEK_SET);
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

int64_t mli_IoMemory_eof(const struct mli_IoMemory *self)
{
        if (self->pos < self->size) {
                return 0;
        } else {
                return EOF;
        }
}

chk_rc mli_IoMemory__write_cstr(struct mli_IoMemory *self, const char *cstr)
{
        /* For testing purposes */
        size_t i = 0;
        while (cstr[i] != '\0') {
                chk(mli_IoMemory_write(
                        (unsigned char *)(&cstr[i]), sizeof(char), 1, self));
                i += 1;
        }

        return CHK_SUCCESS;
chk_error:
        return CHK_FAIL;
}
