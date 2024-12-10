/* Copyright 2018-2023 Sebastian Achim Mueller */
#ifndef MLI_IO_H_
#define MLI_IO_H_

#include <stdint.h>
#include "../string/string.h"

struct mli_IO {
        /* memory */
        unsigned char *cstr;

        /* Capacity of the allocated memory */
        uint64_t capacity;

        /* Size of the payload in the allocated memory */
        uint64_t size;

        /* Position of the cursor */
        uint64_t pos;
};

struct mli_IO mli_IO_init(void);
int mli_IO_close(struct mli_IO *self);
int mli_IO_open(struct mli_IO *self);
size_t mli_IO_write(
        struct mli_IO *self,
        const void *ptr,
        const size_t size,
        const size_t count);
size_t mli_IO_read(
        struct mli_IO *self,
        void *ptr,
        const size_t size,
        const size_t count);
void mli_IO_rewind(struct mli_IO *self);
int64_t mli_IO_tell(struct mli_IO *self);
int64_t mli_IO_seek(
        struct mli_IO *self,
        const int64_t offset,
        const int64_t origin);
int mli_IO_eof(const struct mli_IO *self);

/* to/from path */
int mli_IO_write_from_path(struct mli_IO *self, const char *path);
int mli_IO_read_to_path(struct mli_IO *self, const char *path);

/* internal */
int mli_IO__malloc(struct mli_IO *self);
int mli_IO__malloc_capacity(struct mli_IO *self, const uint64_t capacity);
int mli_IO__realloc_capacity(struct mli_IO *self, const uint64_t new_capacity);
int mli_IO__shrink_to_fit(struct mli_IO *self);
int mli_IO__write_unsigned_char(struct mli_IO *self, const unsigned char *c);
int mli_IO__read_unsigned_char(struct mli_IO *self, unsigned char *c);
#endif
