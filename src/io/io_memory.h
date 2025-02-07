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
chk_rc mli_IoMemory_open(struct mli_IoMemory *self);
size_t mli_IoMemory_write(
        const void *ptr,
        const size_t size,
        const size_t count,
        struct mli_IoMemory *self);
size_t mli_IoMemory_read(
        void *ptr,
        const size_t size,
        const size_t count,
        struct mli_IoMemory *self);
void mli_IoMemory_rewind(struct mli_IoMemory *self);
int64_t mli_IoMemory_tell(struct mli_IoMemory *self);
int64_t mli_IoMemory_seek(
        struct mli_IoMemory *self,
        const int64_t offset,
        const int64_t origin);
int mli_IoMemory_eof(const struct mli_IoMemory *self);

/* internal */
chk_rc mli_IoMemory__malloc(struct mli_IoMemory *self);
chk_rc mli_IoMemory__malloc_capacity(
        struct mli_IoMemory *self,
        const uint64_t capacity);
chk_rc mli_IoMemory__realloc_capacity(
        struct mli_IoMemory *self,
        const uint64_t new_capacity);
chk_rc mli_IoMemory__shrink_to_fit(struct mli_IoMemory *self);
chk_rc mli_IoMemory__write_unsigned_char(
        struct mli_IoMemory *self,
        const unsigned char *c);
chk_rc mli_IoMemory__read_unsigned_char(
        struct mli_IoMemory *self,
        unsigned char *c);
chk_rc mli_IoMemory__write_cstr(struct mli_IoMemory *self, const char *cstr);
#endif
