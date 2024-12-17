/* Copyright 2018-2023 Sebastian Achim Mueller */
#ifndef MLI_IOMEMORY_H_
#define MLI_IOMEMORY_H_

#include <stdint.h>
#include <stddef.h>

struct mli_IoMemory {
        /* memory */
        unsigned char *cstr;

        /* Capacity of the allocated memory */
        uint64_t capacity;

        /* Size of the payload in the allocated memory */
        uint64_t size;

        /* Position of the cursor */
        uint64_t pos;
};

struct mli_IoMemory mli_IoMemory_init(void);
int mli_IoMemory_close(struct mli_IoMemory *self);
int mli_IoMemory_open(struct mli_IoMemory *self);
size_t mli_IoMemory_write(
        struct mli_IoMemory *self,
        const void *ptr,
        const size_t size,
        const size_t count);
size_t mli_IoMemory_read(
        struct mli_IoMemory *self,
        void *ptr,
        const size_t size,
        const size_t count);
void mli_IoMemory_rewind(struct mli_IoMemory *self);
int64_t mli_IoMemory_tell(struct mli_IoMemory *self);
int64_t mli_IoMemory_seek(
        struct mli_IoMemory *self,
        const int64_t offset,
        const int64_t origin);
int mli_IoMemory_eof(const struct mli_IoMemory *self);

/* to/from path */
int mli_IoMemory_write_from_path(struct mli_IoMemory *self, const char *path);
int mli_IoMemory_read_to_path(struct mli_IoMemory *self, const char *path);

/* internal */
int mli_IoMemory__malloc(struct mli_IoMemory *self);
int mli_IoMemory__malloc_capacity(
        struct mli_IoMemory *self,
        const uint64_t capacity);
int mli_IoMemory__realloc_capacity(
        struct mli_IoMemory *self,
        const uint64_t new_capacity);
int mli_IoMemory__shrink_to_fit(struct mli_IoMemory *self);
int mli_IoMemory__write_unsigned_char(
        struct mli_IoMemory *self,
        const unsigned char *c);
int mli_IoMemory__read_unsigned_char(
        struct mli_IoMemory *self,
        unsigned char *c);
int mli_IoMemory__write_cstr(struct mli_IoMemory *self, const char *cstr);
#endif
