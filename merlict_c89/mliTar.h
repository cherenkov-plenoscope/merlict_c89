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

#ifndef MERLICT_C89_MLITAR_H_
#define MERLICT_C89_MLITAR_H_

#include <stdio.h>
#include <stdint.h>

#define MLITAR_TREG '0'
#define MLITAR_TLNK '1'
#define MLITAR_TSYM '2'
#define MLITAR_TCHR '3'
#define MLITAR_TBLK '4'
#define MLITAR_TDIR '5'
#define MLITAR_TFIFO '6'
#define MLITAR_NAME_LENGTH 100

struct mliTarHeader {
        uint64_t mode;
        uint64_t owner;
        uint64_t size;
        uint64_t mtime;
        uint64_t type;
        char name[MLITAR_NAME_LENGTH];
        char linkname[MLITAR_NAME_LENGTH];
};

struct mliTarHeader mliTarHeader_init(void);

struct mliTar {
        FILE *stream;
        uint64_t pos;
        uint64_t remaining_data;
};

struct mliTar mliTar_init(void);
int mliTar_open(struct mliTar *tar, const char *filename, const char *mode);
int mliTar_close(struct mliTar *tar);

int mliTar_read_header(struct mliTar *tar, struct mliTarHeader *h);
int mliTar_read_data(struct mliTar *tar, void *ptr, uint64_t size);

int mliTar_write_header(struct mliTar *tar, const struct mliTarHeader *h);
int mliTar_write_file_header(
        struct mliTar *tar,
        const char *name,
        uint64_t size);
int mliTar_write_dir_header(struct mliTar *tar, const char *name);
int mliTar_write_data(struct mliTar *tar, const void *data, uint64_t size);
int mliTar_finalize(struct mliTar *tar);

#endif
