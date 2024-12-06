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

#ifndef MLI_TAR_H_
#define MLI_TAR_H_

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
uint64_t mli_Tar_round_up(uint64_t n, uint64_t incr);
int mli_Tar_field_to_uint(
        uint64_t *out,
        const char *field,
        const uint64_t fieldsize);
int mli_Tar_uint_to_field(
        const uint64_t value,
        char *field,
        const uint64_t fieldsize);
int mli_Tar_uint64_to_field12_2001star_base256(uint64_t val, char *field);
int mli_Tar_field12_to_uint64_2001star_base256(
        const char *field,
        uint64_t *val);

/* header and raw header */
/* ===================== */
struct mli_TarRawHeader {
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

struct mli_TarHeader {
        uint64_t mode;
        uint64_t owner;
        uint64_t size;
        uint64_t mtime;
        uint64_t type;
        char name[MLI_TAR_NAME_LENGTH];
        char linkname[MLI_TAR_NAME_LENGTH];
};

uint64_t mli_TarRawHeader_checksum(const struct mli_TarRawHeader *rh);
int mli_TarRawHeader_is_null(const struct mli_TarRawHeader *rh);
int mli_TarRawHeader_from_header(
        struct mli_TarRawHeader *rh,
        const struct mli_TarHeader *h);

struct mli_TarHeader mli_TarHeader_init(void);
int mli_TarHeader_set_directory(struct mli_TarHeader *h, const char *name);
int mli_TarHeader_set_normal_file(
        struct mli_TarHeader *h,
        const char *name,
        const uint64_t size);
int mli_TarHeader_from_raw(
        struct mli_TarHeader *h,
        const struct mli_TarRawHeader *rh);

/* tar */
/* === */
struct mli_Tar {
        FILE *stream;
        uint64_t pos;
        uint64_t remaining_data;
};

struct mli_Tar mli_Tar_init(void);

int mli_Tar_read_begin(struct mli_Tar *tar, FILE *file);
int mli_Tar_read_header(struct mli_Tar *tar, struct mli_TarHeader *h);
int mli_Tar_read_data(struct mli_Tar *tar, void *ptr, uint64_t size);
int mli_Tar_read_finalize(struct mli_Tar *tar);

int mli_Tar_write_begin(struct mli_Tar *tar, FILE *file);
int mli_Tar_write_header(struct mli_Tar *tar, const struct mli_TarHeader *h);
int mli_Tar_write_data(struct mli_Tar *tar, const void *data, uint64_t size);
int mli_Tar_write_finalize(struct mli_Tar *tar);

#endif
