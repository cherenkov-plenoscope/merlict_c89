/**
 * Copyright (c) 2017 rxi
 * Copyright (c) 2019 Sebastian A. Mueller
 *                    Max-Planck-Institute for nuclear-physics, Heidelberg
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#ifndef MLITAR_H_
#define MLITAR_H_

#include <stdio.h>
#include <stdint.h>

#define MLI_TAR_VERSION_MAYOR 0
#define MLI_TAR_VERSION_MINOR 1
#define MLI_TAR_VERSION_PATCH 0

#define MLI_TAR_NORMAL_FILE '0'
#define MLI_TAR_HARD_LINK '1'
#define MLI_TAR_SYMBOLIC_LINK '2'
#define MLI_TAR_CHARACTER_SPECIAL '3'
#define MLI_TAR_BLOCK_SPECIAL '4'
#define MLI_TAR_DIRECTORY '5'
#define MLI_TAR_FIFO '6'
#define MLI_TAR_NAME_LENGTH 100

#define MLI_TAR_OCTAL 8u
#define MLI_TAR_MAX_FILESIZE_OCTAL 8589934592lu /* 8^11 */
#define MLI_TAR_HEADER_ONLY_ZEROS -100

struct mliTarHeader {
        uint64_t mode;
        uint64_t owner;
        uint64_t size;
        uint64_t mtime;
        uint64_t type;
        char name[MLI_TAR_NAME_LENGTH];
        char linkname[MLI_TAR_NAME_LENGTH];
};

struct mliTarHeader mliTarHeader_init(void);
int mliTarHeader_set_directory(struct mliTarHeader *h, const char *name);
int mliTarHeader_set_normal_file(
        struct mliTarHeader *h,
        const char *name,
        const uint64_t size);

struct mliTar {
        FILE *stream;
        uint64_t pos;
        uint64_t remaining_data;
};

struct mliTar mliTar_init(void);

int mliTar_begin(struct mliTar *tar, FILE *file);
int mliTar_finalize(struct mliTar *tar);
int mliTar_read_final_zeros(struct mliTar *tar);

int mliTar_open(struct mliTar *tar, const char *path, const char *mode);
int mliTar_close(struct mliTar *tar);

int mliTar_read_header(struct mliTar *tar, struct mliTarHeader *h);
int mliTar_read_data(struct mliTar *tar, void *ptr, uint64_t size);

int mliTar_write_header(struct mliTar *tar, const struct mliTarHeader *h);
int mliTar_write_data(struct mliTar *tar, const void *data, uint64_t size);

/* internal */
int mliTar_uint64_to_field12_2001star_base256(uint64_t val, char *field);
int mliTar_field12_to_uint64_2001star_base256(const char *field, uint64_t *val);
int mliTar_write_null_bytes(struct mliTar *tar, uint64_t n);

#endif
