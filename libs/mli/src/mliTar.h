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

#define MLI_TAR_VERSION_MAYOR 1
#define MLI_TAR_VERSION_MINOR 0
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

/* basics */
/* ====== */
uint64_t mliTar_round_up(uint64_t n, uint64_t incr);
int mliTar_field_to_uint(
        uint64_t *out,
        const char *field,
        const uint64_t fieldsize);
int mliTar_uint_to_field(
        const uint64_t value,
        char *field,
        const uint64_t fieldsize);
int mliTar_uint64_to_field12_2001star_base256(uint64_t val, char *field);
int mliTar_field12_to_uint64_2001star_base256(const char *field, uint64_t *val);

/* header and raw header */
/* ===================== */
struct mliTarRawHeader {
        char name[MLI_TAR_NAME_LENGTH];
        char mode[8];
        char owner[8];
        char group[8];
        char size[12];
        char mtime[12];
        char checksum[8];
        char type;
        char linkname[MLI_TAR_NAME_LENGTH];
        char _padding[255];
};

struct mliTarHeader {
        uint64_t mode;
        uint64_t owner;
        uint64_t size;
        uint64_t mtime;
        uint64_t type;
        char name[MLI_TAR_NAME_LENGTH];
        char linkname[MLI_TAR_NAME_LENGTH];
};

uint64_t mliTarRawHeader_checksum(const struct mliTarRawHeader *rh);
int mliTarRawHeader_is_null(const struct mliTarRawHeader *rh);
int mliTarRawHeader_from_header(
        struct mliTarRawHeader *rh,
        const struct mliTarHeader *h);

struct mliTarHeader mliTarHeader_init(void);
int mliTarHeader_set_directory(struct mliTarHeader *h, const char *name);
int mliTarHeader_set_normal_file(
        struct mliTarHeader *h,
        const char *name,
        const uint64_t size);
int mliTarHeader_from_raw(
        struct mliTarHeader *h,
        const struct mliTarRawHeader *rh);

/* tar */
/* === */
struct mliTar {
        FILE *stream;
        uint64_t pos;
        uint64_t remaining_data;
};

struct mliTar mliTar_init(void);

int mliTar_read_begin(struct mliTar *tar, FILE *file);
int mliTar_read_header(struct mliTar *tar, struct mliTarHeader *h);
int mliTar_read_data(struct mliTar *tar, void *ptr, uint64_t size);
int mliTar_read_finalize(struct mliTar *tar);

int mliTar_write_begin(struct mliTar *tar, FILE *file);
int mliTar_write_header(struct mliTar *tar, const struct mliTarHeader *h);
int mliTar_write_data(struct mliTar *tar, const void *data, uint64_t size);
int mliTar_write_finalize(struct mliTar *tar);

#endif
